#pragma once

#include <CClientAPI.h>
#include <string>
#include <functional>
#include <memory>

namespace Bronnikov {

/**
 * @brief Менеджер EEG-сессий
 *
 * Управляет жизненным циклом CSession, запуском/остановкой записи данных,
 * установкой маркеров активности и управлением состоянием сессии.
 */
class SessionManager {
public:
    SessionManager(clCClient client, clCDevice device);
    ~SessionManager();

    // Запрет копирования
    SessionManager(const SessionManager&) = delete;
    SessionManager& operator=(const SessionManager&) = delete;

    /**
     * @brief Создание и запуск новой сессии
     * @param session_name Имя сессии (для логирования)
     * @return true если сессия успешно создана
     */
    bool startSession(const std::string& session_name);

    /**
     * @brief Остановка текущей сессии (асинхронная)
     * Отправляет команду остановки, но не уничтожает сессию сразу.
     * Сессия будет уничтожена в callback после полной остановки.
     */
    void stopSession();

    /**
     * @brief Уничтожение сессии (вызывается после остановки)
     * ВНИМАНИЕ: Вызывать только после завершения stopSession()!
     */
    void destroySession();

    /**
     * @brief Пауза сессии
     */
    void pauseSession();

    /**
     * @brief Возобновление сессии после паузы
     */
    void resumeSession();

    /**
     * @brief Установка маркера активности
     *
     * Используется для маркировки различных фаз упражнения:
     * - clCUserActivity1: Подготовка
     * - clCUserActivity2: Основная практика
     * - clCUserActivity3: Завершение
     *
     * @param activity Тип активности
     */
    void markActivity(clCUserActivity activity);

    /**
     * @brief Получение текущей сессии
     */
    clCSession getSession() const { return m_session; }

    /**
     * @brief Проверка статуса сессии
     */
    bool isActive() const;
    bool isPaused() const;

    /**
     * @brief Получение длительности сессии в секундах
     */
    double getSessionDuration() const;

    // Callbacks
    using SessionCallback = std::function<void(bool success)>;
    using StateCallback = std::function<void(clCSessionState state)>;

    void setOnSessionStartedCallback(SessionCallback callback) { m_on_session_started = callback; }
    void setOnSessionStoppedCallback(SessionCallback callback) { m_on_session_stopped = callback; }
    void setOnStateChangedCallback(StateCallback callback) { m_on_state_changed = callback; }

private:
    clCClient m_client = nullptr;
    clCDevice m_device = nullptr;
    clCSession m_session = nullptr;

    std::string m_session_name;
    double m_start_time = 0.0;
    double m_pause_time = 0.0;
    double m_total_pause_duration = 0.0;

    SessionCallback m_on_session_started;
    SessionCallback m_on_session_stopped;
    StateCallback m_on_state_changed;

    // Static callbacks для C API (без user_data - ограничение API)
    static void onSessionStartedCallback(clCSession session);
    static void onSessionStoppedCallback(clCSession session);
    static void onSessionErrorCallback(clCSession session);
};

} // namespace Bronnikov
