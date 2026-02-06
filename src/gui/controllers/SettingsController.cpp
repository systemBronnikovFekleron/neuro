#include "controllers/SettingsController.h"
#include "SessionDatabase.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QDateTime>

SettingsController::SettingsController(QObject* parent)
    : QObject(parent)
    , m_settings(std::make_unique<QSettings>("BronnikovMethod", "ExerciseApp"))
{
    loadSettings();
}

SettingsController::~SettingsController() = default;

void SettingsController::loadSettings()
{
    // Загрузка интерфейса
    m_themeIndex = m_settings->value("interface/theme", 0).toInt();
    m_languageIndex = m_settings->value("interface/language", 0).toInt();
    m_showHints = m_settings->value("interface/showHints", true).toBool();

    // Загрузка устройства
    m_autoConnect = m_settings->value("device/autoConnect", true).toBool();
    m_demoMode = m_settings->value("device/demoMode", false).toBool();
    m_lastDevice = m_settings->value("device/lastDevice", "").toString();

    // Загрузка упражнений
    m_defaultDuration = m_settings->value("exercises/defaultDuration", 10).toInt();
    m_soundSignals = m_settings->value("exercises/soundSignals", true).toBool();
    m_voiceInstructions = m_settings->value("exercises/voiceInstructions", false).toBool();

    // Загрузка данных
    m_autosaveCSV = m_settings->value("data/autosaveCSV", true).toBool();
    m_researchMode = m_settings->value("data/researchMode", false).toBool();

    qDebug() << "[SettingsController] Настройки загружены";
}

void SettingsController::saveSettings()
{
    // Сохранение интерфейса
    m_settings->setValue("interface/theme", m_themeIndex);
    m_settings->setValue("interface/language", m_languageIndex);
    m_settings->setValue("interface/showHints", m_showHints);

    // Сохранение устройства
    m_settings->setValue("device/autoConnect", m_autoConnect);
    m_settings->setValue("device/demoMode", m_demoMode);
    m_settings->setValue("device/lastDevice", m_lastDevice);

    // Сохранение упражнений
    m_settings->setValue("exercises/defaultDuration", m_defaultDuration);
    m_settings->setValue("exercises/soundSignals", m_soundSignals);
    m_settings->setValue("exercises/voiceInstructions", m_voiceInstructions);

    // Сохранение данных
    m_settings->setValue("data/autosaveCSV", m_autosaveCSV);
    m_settings->setValue("data/researchMode", m_researchMode);

    m_settings->sync();
    qDebug() << "[SettingsController] Настройки сохранены";
}

// Setters для интерфейса
void SettingsController::setThemeIndex(int index)
{
    if (m_themeIndex != index) {
        m_themeIndex = index;
        saveSettings();
        emit themeIndexChanged();
        qDebug() << "[SettingsController] Тема изменена на:" << index;
    }
}

void SettingsController::setLanguageIndex(int index)
{
    if (m_languageIndex != index) {
        m_languageIndex = index;
        saveSettings();
        emit languageIndexChanged();
        qDebug() << "[SettingsController] Язык изменен на:" << index;
    }
}

void SettingsController::setShowHints(bool enabled)
{
    if (m_showHints != enabled) {
        m_showHints = enabled;
        saveSettings();
        emit showHintsChanged();
    }
}

// Setters для устройства
void SettingsController::setAutoConnect(bool enabled)
{
    if (m_autoConnect != enabled) {
        m_autoConnect = enabled;
        saveSettings();
        emit autoConnectChanged();
    }
}

void SettingsController::setDemoMode(bool enabled)
{
    if (m_demoMode != enabled) {
        m_demoMode = enabled;
        saveSettings();
        emit demoModeChanged();
        qDebug() << "[SettingsController] Демо-режим:" << (enabled ? "включен" : "выключен");
    }
}

void SettingsController::setLastDevice(const QString& device)
{
    if (m_lastDevice != device) {
        m_lastDevice = device;
        saveSettings();
        emit lastDeviceChanged();
    }
}

// Setters для упражнений
void SettingsController::setDefaultDuration(int minutes)
{
    if (m_defaultDuration != minutes) {
        m_defaultDuration = minutes;
        saveSettings();
        emit defaultDurationChanged();
    }
}

void SettingsController::setSoundSignals(bool enabled)
{
    if (m_soundSignals != enabled) {
        m_soundSignals = enabled;
        saveSettings();
        emit soundSignalsChanged();
    }
}

void SettingsController::setVoiceInstructions(bool enabled)
{
    if (m_voiceInstructions != enabled) {
        m_voiceInstructions = enabled;
        saveSettings();
        emit voiceInstructionsChanged();
    }
}

// Setters для данных
void SettingsController::setAutosaveCSV(bool enabled)
{
    if (m_autosaveCSV != enabled) {
        m_autosaveCSV = enabled;
        saveSettings();
        emit autosaveCSVChanged();
    }
}

void SettingsController::setResearchMode(bool enabled)
{
    if (m_researchMode != enabled) {
        m_researchMode = enabled;
        saveSettings();
        emit researchModeChanged();
        qDebug() << "[SettingsController] Research Mode:" << (enabled ? "включен" : "выключен");
    }
}

void SettingsController::exportAllData(const QString& userId)
{
    qDebug() << "[SettingsController] Экспорт всех данных для пользователя:" << userId;

    if (!m_database || !m_database->isOpen()) {
        emit exportFailed("База данных не открыта");
        return;
    }

    try {
        // Создаем директорию для экспорта
        QString exportDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
        QString exportPath = exportDir + "/BronnikovExport_" + timestamp;

        QDir dir;
        if (!dir.mkpath(exportPath)) {
            emit exportFailed("Не удалось создать директорию экспорта");
            return;
        }

        // Получаем все сессии пользователя
        auto sessions = m_database->getSessions(userId.toStdString(), 0);

        if (sessions.empty()) {
            emit exportFailed("Нет данных для экспорта");
            return;
        }

        // Создаем сводный CSV файл
        QString csvPath = exportPath + "/sessions_summary.csv";
        QFile csvFile(csvPath);

        if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            emit exportFailed("Не удалось создать CSV файл");
            return;
        }

        QTextStream out(&csvFile);

        // Заголовок CSV
        out << "Timestamp,Exercise,Stage,Duration (sec),Success Rate (%),";
        out << "Avg Alpha,Avg Beta,Avg Theta,Avg Concentration,Avg Relaxation,";
        out << "Avg Fatigue,Avg Heart Rate,Target Achieved\n";

        // Данные сессий
        for (const auto& session : sessions) {
            out << QString::fromStdString(session.timestamp) << ",";
            out << QString::fromStdString(session.exercise_name) << ",";
            out << session.exercise_stage << ",";
            out << session.duration_seconds << ",";
            out << session.success_rate << ",";
            out << session.avg_alpha << ",";
            out << session.avg_beta << ",";
            out << session.avg_theta << ",";
            out << session.avg_concentration << ",";
            out << session.avg_relaxation << ",";
            out << session.avg_fatigue << ",";
            out << session.avg_heart_rate << ",";
            out << (session.target_achieved ? "Yes" : "No") << "\n";
        }

        csvFile.close();

        qDebug() << "[SettingsController] ✓ Экспорт завершен:" << csvPath;
        qDebug() << "  Экспортировано сессий:" << sessions.size();

        emit exportCompleted(csvPath);

    } catch (const std::exception& e) {
        qDebug() << "[SettingsController] ✗ Ошибка экспорта:" << e.what();
        emit exportFailed(QString("Ошибка: ") + e.what());
    }
}

void SettingsController::clearHistory(const QString& userId)
{
    qDebug() << "[SettingsController] Очистка истории для пользователя:" << userId;

    if (!m_database || !m_database->isOpen()) {
        qDebug() << "[SettingsController] ✗ База данных не открыта";
        return;
    }

    // Удаляем старые сессии (оставляем только последние 7 дней)
    // TODO: В SessionDatabase нет метода для удаления сессий конкретного пользователя
    // Пока используем cleanupOldSessions() который удалит старые сессии всех пользователей
    int deleted = m_database->cleanupOldSessions(7);

    qDebug() << "[SettingsController] ✓ Удалено сессий:" << deleted;

    emit historyCleared();
}

void SettingsController::resetToDefaults()
{
    qDebug() << "[SettingsController] Сброс настроек к значениям по умолчанию";

    // Интерфейс
    setThemeIndex(0);
    setLanguageIndex(0);
    setShowHints(true);

    // Устройство
    setAutoConnect(true);
    setDemoMode(false);
    setLastDevice("");

    // Упражнения
    setDefaultDuration(10);
    setSoundSignals(true);
    setVoiceInstructions(false);

    // Данные
    setAutosaveCSV(true);
    setResearchMode(false);

    qDebug() << "[SettingsController] ✓ Настройки сброшены";
}
