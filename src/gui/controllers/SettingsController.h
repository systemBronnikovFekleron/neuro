#pragma once

#include <QObject>
#include <QString>
#include <QSettings>
#include <memory>

// Forward declarations
namespace Bronnikov {
    class SessionDatabase;
}

/**
 * @brief Qt Controller для управления настройками приложения
 *
 * Использует QSettings для сохранения настроек между сеансами.
 * Предоставляет QML интерфейс для всех настроек приложения.
 */
class SettingsController : public QObject
{
    Q_OBJECT

    // Настройки интерфейса
    Q_PROPERTY(int themeIndex READ themeIndex WRITE setThemeIndex NOTIFY themeIndexChanged)
    Q_PROPERTY(int languageIndex READ languageIndex WRITE setLanguageIndex NOTIFY languageIndexChanged)
    Q_PROPERTY(bool showHints READ showHints WRITE setShowHints NOTIFY showHintsChanged)

    // Настройки устройства
    Q_PROPERTY(bool autoConnect READ autoConnect WRITE setAutoConnect NOTIFY autoConnectChanged)
    Q_PROPERTY(bool demoMode READ demoMode WRITE setDemoMode NOTIFY demoModeChanged)
    Q_PROPERTY(QString lastDevice READ lastDevice WRITE setLastDevice NOTIFY lastDeviceChanged)

    // Настройки упражнений
    Q_PROPERTY(int defaultDuration READ defaultDuration WRITE setDefaultDuration NOTIFY defaultDurationChanged)
    Q_PROPERTY(bool soundSignals READ soundSignals WRITE setSoundSignals NOTIFY soundSignalsChanged)
    Q_PROPERTY(bool voiceInstructions READ voiceInstructions WRITE setVoiceInstructions NOTIFY voiceInstructionsChanged)

    // Настройки данных
    Q_PROPERTY(bool autosaveCSV READ autosaveCSV WRITE setAutosaveCSV NOTIFY autosaveCSVChanged)
    Q_PROPERTY(bool researchMode READ researchMode WRITE setResearchMode NOTIFY researchModeChanged)

public:
    explicit SettingsController(QObject* parent = nullptr);
    ~SettingsController() override;

    // Getters для интерфейса
    int themeIndex() const { return m_themeIndex; }
    int languageIndex() const { return m_languageIndex; }
    bool showHints() const { return m_showHints; }

    // Getters для устройства
    bool autoConnect() const { return m_autoConnect; }
    bool demoMode() const { return m_demoMode; }
    QString lastDevice() const { return m_lastDevice; }

    // Getters для упражнений
    int defaultDuration() const { return m_defaultDuration; }
    bool soundSignals() const { return m_soundSignals; }
    bool voiceInstructions() const { return m_voiceInstructions; }

    // Getters для данных
    bool autosaveCSV() const { return m_autosaveCSV; }
    bool researchMode() const { return m_researchMode; }

    // Setters для интерфейса
    void setThemeIndex(int index);
    void setLanguageIndex(int index);
    void setShowHints(bool enabled);

    // Setters для устройства
    void setAutoConnect(bool enabled);
    void setDemoMode(bool enabled);
    void setLastDevice(const QString& device);

    // Setters для упражнений
    void setDefaultDuration(int minutes);
    void setSoundSignals(bool enabled);
    void setVoiceInstructions(bool enabled);

    // Setters для данных
    void setAutosaveCSV(bool enabled);
    void setResearchMode(bool enabled);

    // Q_INVOKABLE методы для вызова из QML
    Q_INVOKABLE void exportAllData(const QString& userId = "default");
    Q_INVOKABLE void clearHistory(const QString& userId = "default");
    Q_INVOKABLE void resetToDefaults();

    // Установка ссылки на базу данных
    void setDatabase(Bronnikov::SessionDatabase* db) { m_database = db; }

signals:
    // Сигналы изменения интерфейса
    void themeIndexChanged();
    void languageIndexChanged();
    void showHintsChanged();

    // Сигналы изменения устройства
    void autoConnectChanged();
    void demoModeChanged();
    void lastDeviceChanged();

    // Сигналы изменения упражнений
    void defaultDurationChanged();
    void soundSignalsChanged();
    void voiceInstructionsChanged();

    // Сигналы изменения данных
    void autosaveCSVChanged();
    void researchModeChanged();

    // Уведомления о действиях
    void exportCompleted(const QString& path);
    void exportFailed(const QString& error);
    void historyCleared();

private:
    // QSettings для хранения настроек
    std::unique_ptr<QSettings> m_settings;

    // Настройки интерфейса
    int m_themeIndex = 0;          // 0=Светлая, 1=Темная, 2=Авто
    int m_languageIndex = 0;       // 0=Русский, 1=English
    bool m_showHints = true;

    // Настройки устройства
    bool m_autoConnect = true;
    bool m_demoMode = false;
    QString m_lastDevice;

    // Настройки упражнений
    int m_defaultDuration = 10;    // минуты
    bool m_soundSignals = true;
    bool m_voiceInstructions = false;

    // Настройки данных
    bool m_autosaveCSV = true;
    bool m_researchMode = false;  // Research mode - экспорт raw EEG/PPG/MEMS

    // Ссылка на базу данных
    Bronnikov::SessionDatabase* m_database = nullptr;

    // Внутренние методы
    void loadSettings();
    void saveSettings();
};
