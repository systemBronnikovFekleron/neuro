#include "CapsuleManager.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <map>

namespace Bronnikov {

// Глобальное хранилище для device list (временное решение для callback)
static std::vector<std::string> g_discovered_devices;
static bool g_discovery_complete = false;

// Глобальные маппинги для связи handle → CapsuleManager
static std::map<clCClient, CapsuleManager*> g_client_map;
static std::map<clCDevice, CapsuleManager*> g_device_map;
static std::map<clCDeviceLocator, CapsuleManager*> g_locator_map;

CapsuleManager::CapsuleManager() {
}

CapsuleManager::~CapsuleManager() {
    disconnect();
}

bool CapsuleManager::connect(const std::string& connection_url) {
    // Создание клиента (принимает const char* напрямую)
    m_client = clCClient_CreateWithName("BronnikovExerciseApp");

    if (!m_client) {
        if (m_on_error) {
            m_on_error("Не удалось создать CClient");
        }
        return false;
    }

    // Установка callback'ов (без user_data - ограничение API)
    clCClientDelegate on_connected = clCClient_GetOnConnectedEvent(m_client);
    clCClientDelegate_Set(on_connected, onClientConnectedCallback);

    clCClientDelegateDisconnectReason on_disconnected = clCClient_GetOnDisconnectedEvent(m_client);
    clCClientDelegateDisconnectReason_Set(on_disconnected, onClientDisconnectedCallback);

    clCClientDelegateError on_error = clCClient_GetOnErrorEvent(m_client);
    clCClientDelegateError_Set(on_error, onErrorCallback);

    // Регистрируем в глобальном маппинге
    g_client_map[m_client] = this;

    // Подключение (принимает const char* напрямую)
    clCClient_Connect(m_client, connection_url.c_str());

    return true;
}

void CapsuleManager::disconnect() {
    if (m_device) {
        disconnectDevice();
    }

    if (m_locator) {
        g_locator_map.erase(m_locator);
        clCDeviceLocator_Destroy(m_locator);
        m_locator = nullptr;
    }

    if (m_client) {
        // Удаляем из глобального маппинга
        g_client_map.erase(m_client);

        clCClient_Disconnect(m_client);
        clCClient_Destroy(m_client);
        m_client = nullptr;
    }

    m_connected = false;
}

std::vector<std::string> CapsuleManager::discoverDevices() {
    g_discovered_devices.clear();
    g_discovery_complete = false;

    if (!m_client || !m_connected) {
        if (m_on_error) {
            m_on_error("Клиент не подключен");
        }
        return {};
    }

    // Создание локатора устройств через clCClient_ChooseDeviceType
    if (!m_locator) {
        m_locator = clCClient_ChooseDeviceType(m_client, clC_DT_NeiryBand);
        if (!m_locator) {
            if (m_on_error) {
                m_on_error("Не удалось создать DeviceLocator");
            }
            return {};
        }

        // Регистрация в глобальном map для callback
        g_locator_map[m_locator] = this;

        // Установка callback для получения списка устройств
        clCDeviceLocatorDelegateDeviceInfoList devices_delegate =
            clCDeviceLocator_GetOnDevicesEvent(m_locator);
        clCDeviceLocatorDelegateDeviceInfoList_Set(devices_delegate, onDevicesFoundCallback);
    }

    // Запуск асинхронного поиска на 5 секунд
    clCDeviceLocator_RequestDevices(m_locator, 5);

    // НОВОЕ: Возвращаем пустой вектор сразу, callback уведомит через m_on_devices_found
    // Это убирает блокирующий цикл и делает операцию полностью асинхронной
    std::cout << "[CapsuleManager] Поиск устройств запущен асинхронно (5 сек)" << std::endl;
    return {};
}

bool CapsuleManager::connectToDevice(const std::string& device_id) {
    if (!m_locator) {
        if (m_on_error) {
            m_on_error("DeviceLocator не инициализирован");
        }
        return false;
    }

    // Если устройство уже создано и подключено, не создаём заново
    if (m_device && m_device_connected) {
        std::cout << "[CapsuleManager] Устройство уже подключено" << std::endl;
        return true;
    }

    // Если устройство создано но не подключено, просто подключаемся
    if (m_device) {
        std::cout << "[CapsuleManager] Устройство уже создано, подключаемся..." << std::endl;
        clCDevice_Connect(m_device);
        return true;
    }

    std::cout << "[CapsuleManager] Создание устройства с ID: " << device_id << std::endl;

    // Даем время на завершение поиска и обработку данных
    for (int i = 0; i < 10; ++i) {
        if (m_client) {
            clCClient_Update(m_client);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    // Создание устройства по ID
    m_device = clCDeviceLocator_CreateDevice(m_locator, device_id.c_str());
    if (!m_device) {
        std::cout << "[CapsuleManager] ❌ clCDeviceLocator_CreateDevice вернул nullptr для ID: " << device_id << std::endl;
        if (m_on_error) {
            m_on_error("Не удалось создать устройство");
        }
        return false;
    }

    std::cout << "[CapsuleManager] ✓ Устройство создано успешно" << std::endl;

    // Регистрируем устройство в глобальном map
    g_device_map[m_device] = this;

    // Установка callback для изменения состояния подключения
    clCDeviceDelegateDeviceConnectionState conn_delegate =
        clCDevice_GetOnConnectionStateChangedEvent(m_device);
    clCDeviceDelegateConnectionState_Set(conn_delegate, onDeviceConnectionStateChangedCallback);

    // Установка callback для обновления уровня батареи
    clCDeviceDelegateUChar battery_delegate =
        clCDevice_GetOnBatteryChargeEvent(m_device);
    clCDeviceDelegateBatteryCharge_Set(battery_delegate, onBatteryUpdateCallback);

    // Получаем начальный уровень батареи
    m_battery_level = clCDevice_GetBatteryCharge(m_device);

    // Подключение к устройству (асинхронное, void return)
    clCDevice_Connect(m_device);

    return true;
}

void CapsuleManager::disconnectDevice() {
    if (m_device) {
        // Удаляем из глобального map
        g_device_map.erase(m_device);

        clCDevice_Disconnect(m_device);
        // НЕТ Destroy для device - управляется автоматически
        m_device = nullptr;
        std::cout << "[CapsuleManager] Устройство отключено" << std::endl;
    }
    m_device_connected = false;

    // НЕ уничтожаем локатор - он нужен для повторного поиска
    // CapsuleAPI не позволяет создать новый локатор после уничтожения старого
}

void CapsuleManager::update() {
    if (m_client) {
        clCClient_Update(m_client);
    }
}

int CapsuleManager::getBatteryLevel() const {
    // Возвращаем кэшированное значение, обновляемое через callback
    return m_battery_level;
}

int CapsuleManager::getSignalQuality() const {
    if (!m_device) {
        return 0;
    }

    // TODO: Реализовать получение качества сигнала через сопротивление электродов
    return 100;  // Заглушка
}

void CapsuleManager::switchDeviceMode(clCDeviceMode mode) {
    if (!m_device) {
        std::cout << "[CapsuleManager] ❌ Устройство не подключено, не могу переключить режим" << std::endl;
        return;
    }

    const char* mode_name = "Unknown";
    switch (mode) {
        case clC_DM_Signal: mode_name = "Signal"; break;
        case clC_DM_StartPPG: mode_name = "PPG"; break;
        case clC_DM_StartMEMS: mode_name = "MEMS"; break;
        case clC_DM_Idle: mode_name = "Idle"; break;
        case clC_DM_PowerDown: mode_name = "PowerDown"; break;
        case clC_DM_Resistance: mode_name = "Resistance"; break;
        default: break;
    }

    std::cout << "[CapsuleManager] Переключение режима устройства на " << mode_name << "..." << std::endl;
    clCDevice_SwitchMode(m_device, mode);
}

// Static callbacks (без user_data - ограничение API)
void CapsuleManager::onClientConnectedCallback(clCClient client) {
    std::cout << "[CapsuleManager] ✓ Подключено к Capsule backend" << std::endl;

    // Используем глобальный маппинг для получения manager
    auto it = g_client_map.find(client);
    if (it != g_client_map.end()) {
        CapsuleManager* manager = it->second;
        manager->m_connected = true;

        if (manager->m_on_connected) {
            manager->m_on_connected(true);
        }
    }
}

void CapsuleManager::onClientDisconnectedCallback(clCClient client, clCDisconnectReason reason) {
    const char* reason_str = "Unknown";
    switch (reason) {
        case clC_DR_UserRequested: reason_str = "По запросу пользователя"; break;
        case clC_DR_Destruction: reason_str = "Уничтожение объекта"; break;
        case clC_DR_FatalError: reason_str = "Фатальная ошибка"; break;
    }
    std::cout << "[CapsuleManager] ✗ Отключено от Capsule: " << reason_str << std::endl;

    // Используем глобальный маппинг
    auto it = g_client_map.find(client);
    if (it != g_client_map.end()) {
        CapsuleManager* manager = it->second;
        manager->m_connected = false;

        if (manager->m_on_connected) {
            manager->m_on_connected(false);
        }
    }
}

void CapsuleManager::onErrorCallback(clCClient client, clCError error) {
    const char* error_str = "Unknown error";
    switch (error) {
        case clC_Error_OK: error_str = "OK"; break;
        case clC_Error_FailedToConnect: error_str = "Failed to connect"; break;
        case clC_Error_FailedToInitConnection: error_str = "Failed to initialize connection"; break;
        case clC_Error_FailedToInitialize: error_str = "Failed to initialize"; break;
        case clC_Error_DeviceError: error_str = "Device error"; break;
        case clC_Error_IndividualNFBNotCalibrated: error_str = "Individual NFB not calibrated"; break;
        case clC_Error_NotReceived: error_str = "Not received"; break;
        case clC_Error_UNKNOWN: error_str = "Unknown"; break;
    }
    std::cout << "[CapsuleManager] ✗ Ошибка: " << error_str << std::endl;
}

void CapsuleManager::onDevicesFoundCallback(clCDeviceLocator locator, clCDeviceInfoList device_list) {
    std::cout << "[CapsuleManager] Получен список устройств" << std::endl;

    g_discovered_devices.clear();

    size_t count = clCDeviceInfoList_GetCount(device_list);
    std::cout << "[CapsuleManager] Найдено устройств: " << count << std::endl;

    for (size_t i = 0; i < count; ++i) {
        clCDeviceInfo info = clCDeviceInfoList_GetDeviceInfo(device_list, i);
        clCString id_str = clCDeviceInfo_GetID(info);
        clCString name_str = clCDeviceInfo_GetName(info);

        std::string device_id = clCString_CStr(id_str);
        std::string device_name = clCString_CStr(name_str);

        std::cout << "  [" << i + 1 << "] " << device_name << " (ID: " << device_id << ")" << std::endl;

        g_discovered_devices.push_back(device_id);

        clCString_Free(id_str);
        clCString_Free(name_str);
    }

    g_discovery_complete = true;

    // Уведомляем через callback о найденных устройствах
    auto it = g_locator_map.find(locator);
    if (it != g_locator_map.end() && it->second) {
        CapsuleManager* manager = it->second;
        if (manager->m_on_devices_found) {
            manager->m_on_devices_found(g_discovered_devices);
        }
    }
}

void CapsuleManager::onDeviceConnectionStateChangedCallback(clCDevice device, clCDeviceConnectionState state) {
    const char* state_str = "Unknown";
    switch (state) {
        case clC_SE_Disconnected: state_str = "Отключено"; break;
        case clC_SE_Connected: state_str = "Подключено"; break;
        case clC_SE_UnsupportedConnection: state_str = "Неподдерживаемое подключение"; break;
    }

    std::cout << "[CapsuleManager] Состояние устройства: " << state_str << std::endl;

    // Находим CapsuleManager через глобальный map
    auto it = g_device_map.find(device);
    if (it != g_device_map.end() && it->second) {
        CapsuleManager* manager = it->second;

        if (state == clC_SE_Connected) {
            manager->m_device_connected = true;

            if (manager->m_on_device_connected) {
                // Получаем ID устройства для callback
                std::string device_id = "Unknown";
                manager->m_on_device_connected(device_id, true);
            }
        } else if (state == clC_SE_Disconnected) {
            manager->m_device_connected = false;

            if (manager->m_on_device_connected) {
                std::string device_id = "Unknown";
                manager->m_on_device_connected(device_id, false);
            }
        }
    }
}

void CapsuleManager::onBatteryUpdateCallback(clCDevice device, clCUChar battery_level) {
    // Находим CapsuleManager через глобальный map
    auto it = g_device_map.find(device);
    if (it != g_device_map.end() && it->second) {
        CapsuleManager* manager = it->second;

        // Обновляем кэшированное значение (clCUChar это unsigned char, 0-100)
        manager->m_battery_level = static_cast<int>(battery_level);

        // Вызываем пользовательский callback (если установлен)
        if (manager->m_on_battery_update) {
            manager->m_on_battery_update(static_cast<int>(battery_level));
        }
    }
}

} // namespace Bronnikov
