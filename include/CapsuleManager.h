#pragma once

#include <CClientAPI.h>
#include <string>
#include <functional>
#include <memory>

namespace Bronnikov {

/**
 * @brief Менеджер подключения к CapsuleAPI
 *
 * Управляет жизненным циклом CClient, подключением к устройству Neiry
 * и инициализацией всех необходимых компонентов.
 */
class CapsuleManager {
public:
    CapsuleManager();
    ~CapsuleManager();

    // Запрет копирования
    CapsuleManager(const CapsuleManager&) = delete;
    CapsuleManager& operator=(const CapsuleManager&) = delete;

    /**
     * @brief Подключение к Capsule backend
     * @param connection_url URL подключения (по умолчанию "inproc://capsule")
     * @return true если подключение успешно
     */
    bool connect(const std::string& connection_url = "inproc://capsule");

    /**
     * @brief Отключение от Capsule backend
     */
    void disconnect();

    /**
     * @brief Поиск доступных устройств Neiry
     * @return Список найденных устройств
     */
    std::vector<std::string> discoverDevices();

    /**
     * @brief Подключение к конкретному устройству
     * @param device_name Имя устройства
     * @return true если подключение успешно
     */
    bool connectToDevice(const std::string& device_name);

    /**
     * @brief Отключение от устройства
     */
    void disconnectDevice();

    /**
     * @brief Обновление состояния клиента (вызывать регулярно)
     *
     * Обрабатывает все pending события и callbacks.
     * Рекомендуется вызывать каждые 50ms.
     */
    void update();

    /**
     * @brief Проверка статуса подключения
     */
    bool isConnected() const { return m_connected; }
    bool isDeviceConnected() const { return m_device_connected; }

    /**
     * @brief Получение текущего устройства
     */
    clCDevice getDevice() const { return m_device; }

    /**
     * @brief Получение клиента Capsule
     */
    clCClient getClient() const { return m_client; }

    /**
     * @brief Получение информации о батарее устройства
     * @return Уровень заряда в процентах (0-100)
     */
    int getBatteryLevel() const;

    /**
     * @brief Получение качества сигнала (сопротивление электродов)
     * @return Качество сигнала (0-100, где 100 - отличное качество)
     */
    int getSignalQuality() const;

    /**
     * @brief Переключение режима устройства
     * @param mode Режим устройства (Signal, PPG, MEMS, Idle, etc.)
     */
    void switchDeviceMode(clCDeviceMode mode);

    // Callbacks
    using ConnectionCallback = std::function<void(bool success)>;
    using DeviceCallback = std::function<void(const std::string& device_name, bool success)>;
    using ErrorCallback = std::function<void(const std::string& error)>;
    using BatteryCallback = std::function<void(int level)>;

    void setOnConnectedCallback(ConnectionCallback callback) { m_on_connected = callback; }
    void setOnDeviceConnectedCallback(DeviceCallback callback) { m_on_device_connected = callback; }
    void setOnErrorCallback(ErrorCallback callback) { m_on_error = callback; }
    void setOnBatteryUpdateCallback(BatteryCallback callback) { m_on_battery_update = callback; }

private:
    clCClient m_client = nullptr;
    clCDeviceLocator m_locator = nullptr;
    clCDevice m_device = nullptr;

    bool m_connected = false;
    bool m_device_connected = false;
    int m_battery_level = 0;

    ConnectionCallback m_on_connected;
    DeviceCallback m_on_device_connected;
    ErrorCallback m_on_error;
    BatteryCallback m_on_battery_update;

    // Static callbacks для C API (без user_data - ограничение API)
    static void onClientConnectedCallback(clCClient client);
    static void onClientDisconnectedCallback(clCClient client, clCDisconnectReason reason);
    static void onErrorCallback(clCClient client, clCError error);
    static void onDevicesFoundCallback(clCDeviceLocator locator, clCDeviceInfoList device_list);
    static void onDeviceConnectionStateChangedCallback(clCDevice device, clCDeviceConnectionState state);
    static void onBatteryUpdateCallback(clCDevice device, clCUChar battery_level);
};

} // namespace Bronnikov
