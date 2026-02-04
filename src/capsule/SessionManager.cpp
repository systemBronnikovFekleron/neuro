#include "SessionManager.h"
#include <iostream>
#include <chrono>
#include <map>

namespace Bronnikov {

// Глобальный маппинг для связи handle → объект (из-за отсутствия user_data в API)
static std::map<clCSession, SessionManager*> g_session_map;

SessionManager::SessionManager(clCClient client, clCDevice device)
    : m_client(client)
    , m_device(device)
{
}

SessionManager::~SessionManager() {
    // В деструкторе безопасно уничтожить сессию напрямую
    destroySession();
}

bool SessionManager::startSession(const std::string& session_name) {
    if (!m_client || !m_device) {
        return false;
    }

    m_session_name = session_name;

    // Создание сессии через клиента
    m_session = clCClient_CreateSession(m_client, m_device);
    if (!m_session) {
        return false;
    }

    // Регистрируем сессию в глобальном map
    g_session_map[m_session] = this;

    // Установка callbacks (без user_data - ограничение API)
    clCSessionDelegate on_started = clCSession_GetOnSessionStartedEvent(m_session);
    clCSessionDelegate_Set(on_started, onSessionStartedCallback);

    clCSessionDelegate on_stopped = clCSession_GetOnSessionStoppedEvent(m_session);
    clCSessionDelegate_Set(on_stopped, onSessionStoppedCallback);

    // ПРИМЕЧАНИЕ: В текущей версии API нет отдельного события для ошибок сессии
    // Ошибки будут обрабатываться через другие механизмы

    // Запуск сессии (void return, асинхронный)
    clCSession_Start(m_session);

    m_start_time = std::chrono::duration<double>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    std::cout << "[SessionManager] Запрос на запуск сессии отправлен..." << std::endl;

    return true;
}

void SessionManager::stopSession() {
    if (m_session) {
        // Отправляем команду остановки (асинхронная)
        clCSession_Stop(m_session);

        // ВАЖНО: НЕ вызываем Destroy немедленно!
        // Destroy будет вызван из callback onSessionStoppedCallback
        // или из деструктора после завершения всех операций

        // НЕ удаляем из map пока callback не отработает
        // g_session_map.erase(m_session);
    }
}

void SessionManager::destroySession() {
    if (m_session) {
        // Удаляем из глобального map
        g_session_map.erase(m_session);

        // Теперь безопасно уничтожить
        clCSession_Destroy(m_session);
        m_session = nullptr;
    }
}

void SessionManager::pauseSession() {
    // ПРИМЕЧАНИЕ: API CapsuleAPI не поддерживает pause/resume
    // Оставляем заглушку для будущей реализации или эмуляции через Stop/Start
    if (m_session && isActive()) {
        m_pause_time = std::chrono::duration<double>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();

        // TODO: В текущей версии API нет Pause, можно эмулировать через Stop
        // clCSession_Stop(m_session);
    }
}

void SessionManager::resumeSession() {
    // ПРИМЕЧАНИЕ: API CapsuleAPI не поддерживает pause/resume
    // Оставляем заглушку для будущей реализации
    if (m_session && isPaused()) {
        double now = std::chrono::duration<double>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();

        m_total_pause_duration += (now - m_pause_time);
        m_pause_time = 0.0;

        // TODO: В текущей версии API нет Resume, можно эмулировать через Start
        // clCSession_Start(m_session);
    }
}

void SessionManager::markActivity(clCUserActivity activity) {
    if (m_session) {
        clCSession_MarkActivity(m_session, activity);
    }
}

bool SessionManager::isActive() const {
    if (!m_session) {
        return false;
    }

    return clCSession_IsActive(m_session);
}

bool SessionManager::isPaused() const {
    if (!m_session) {
        return false;
    }

    clCSessionState state = clCSession_GetSessionState(m_session);
    return state == clC_SS_Paused || state == clC_SS_Pausing;
}

double SessionManager::getSessionDuration() const {
    if (!m_session || m_start_time == 0.0) {
        return 0.0;
    }

    double now = std::chrono::duration<double>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    return (now - m_start_time) - m_total_pause_duration;
}

// Static callbacks (без user_data - ограничение API)
void SessionManager::onSessionStartedCallback(clCSession session) {
    std::cout << "[SessionManager] ✓ Сессия начата" << std::endl;

    // Находим объект SessionManager через глобальный map
    auto it = g_session_map.find(session);
    if (it != g_session_map.end() && it->second) {
        SessionManager* manager = it->second;
        if (manager->m_on_session_started) {
            manager->m_on_session_started(true);
        }
    }
}

void SessionManager::onSessionStoppedCallback(clCSession session) {
    std::cout << "[SessionManager] ✓ Сессия остановлена" << std::endl;

    // Находим объект SessionManager через глобальный map
    auto it = g_session_map.find(session);
    if (it != g_session_map.end() && it->second) {
        SessionManager* manager = it->second;
        if (manager->m_on_session_stopped) {
            manager->m_on_session_stopped(true);
        }
    }
}

void SessionManager::onSessionErrorCallback(clCSession session) {
    std::cerr << "[SessionManager] ✗ Ошибка сессии" << std::endl;

    // Находим объект SessionManager через глобальный map
    auto it = g_session_map.find(session);
    if (it != g_session_map.end() && it->second) {
        SessionManager* manager = it->second;
        if (manager->m_on_session_started) {
            // При ошибке вызываем callback с false
            manager->m_on_session_started(false);
        }
    }
}

} // namespace Bronnikov
