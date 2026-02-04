/**
 * @file main_gui.cpp
 * @brief Точка входа для GUI версии BronnikovExerciseApp (Qt/QML)
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QDebug>

// Qt Models
#include "models/ExerciseModel.h"
#include "models/MetricsModel.h"
#include "models/SessionModel.h"

// Qt Controllers
#include "controllers/DeviceController.h"
#include "controllers/ExerciseController.h"

// C++ Business Logic
#include "CapsuleManager.h"
#include "SessionManager.h"
#include "MetricsCollector.h"
#include "SessionDatabase.h"

int main(int argc, char *argv[])
{
    // Включить масштабирование для High DPI дисплеев
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    // Настройка приложения
    app.setOrganizationName("Bronnikov Method");
    app.setOrganizationDomain("bronnikov.com");
    app.setApplicationName("Bronnikov Exercise App");
    app.setApplicationVersion("0.10.0");

    qDebug() << "==============================================";
    qDebug() << "Bronnikov Exercise App - GUI Version";
    qDebug() << "==============================================";
    qDebug() << "Version:" << app.applicationVersion();
    qDebug() << "Qt Version:" << QT_VERSION_STR;
    qDebug() << "==============================================";

    // Создание QML движка
    QQmlApplicationEngine engine;

    // ============================================================================
    // Инициализация C++ бизнес-логики
    // ============================================================================

    // Database
    auto database = std::make_shared<Bronnikov::SessionDatabase>("data/bronnikov_sessions.db");
    if (!database->open()) {
        qCritical() << "Failed to open database!";
        return -1;
    }

    // CapsuleAPI Manager (подключение к устройству)
    auto capsuleManager = std::make_shared<Bronnikov::CapsuleManager>();

    // Session Manager and Metrics Collector will be initialized after device connection
    // For now, set to nullptr (controllers will handle nullptr checks)
    std::shared_ptr<Bronnikov::SessionManager> sessionManager = nullptr;
    std::shared_ptr<Bronnikov::MetricsCollector> metricsCollector = nullptr;

    // ============================================================================
    // Создание Qt моделей для QML
    // ============================================================================

    // Exercise Model - список упражнений
    auto exerciseModel = new ExerciseModel(&app);
    exerciseModel->loadExercises();

    // Metrics Model - real-time метрики
    auto metricsModel = new MetricsModel(&app);

    // Session Model - история и статистика
    auto sessionModel = new SessionModel(&app);
    sessionModel->setDatabase(database.get());
    sessionModel->loadStatistics();
    sessionModel->loadStageProgress();

    // ============================================================================
    // Создание Qt контроллеров для QML
    // ============================================================================

    // Device Controller - управление подключением к устройству
    auto deviceController = new DeviceController(&app);
    deviceController->setCapsuleManager(capsuleManager.get());

    // Связываем MetricsModel с DeviceController для получения реальных данных
    metricsModel->setDeviceController(deviceController);

    // Exercise Controller - управление выполнением упражнений
    auto exerciseController = new ExerciseController(&app);
    if (sessionManager) {
        exerciseController->setSessionManager(sessionManager.get());
    }
    if (metricsCollector) {
        exerciseController->setMetricsCollector(metricsCollector.get());
    }
    exerciseController->setDatabase(database.get());

    // Связь контроллеров
    QObject::connect(deviceController, &DeviceController::sessionStarted,
                     metricsModel, &MetricsModel::startMonitoring);
    QObject::connect(deviceController, &DeviceController::sessionStopped,
                     metricsModel, &MetricsModel::stopMonitoring);

    QObject::connect(exerciseController, &ExerciseController::exerciseCompleted,
                     sessionModel, [sessionModel]() {
                         sessionModel->loadStatistics();
                         sessionModel->loadStageProgress();
                         sessionModel->loadRecentSessions();
                     });

    // ============================================================================
    // Регистрация моделей и контроллеров в QML контексте
    // ============================================================================

    QQmlContext* rootContext = engine.rootContext();

    // Models
    rootContext->setContextProperty("exerciseModel", exerciseModel);
    rootContext->setContextProperty("metricsModel", metricsModel);
    rootContext->setContextProperty("sessionModel", sessionModel);

    // Controllers
    rootContext->setContextProperty("deviceController", deviceController);
    rootContext->setContextProperty("exerciseController", exerciseController);

    // ============================================================================
    // Загрузка главного QML файла
    // ============================================================================

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            qCritical() << "Failed to load QML!";
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);

    engine.load(url);

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "No root objects loaded from QML!";
        return -1;
    }

    qDebug() << "GUI initialized successfully";
    qDebug() << "==============================================";

    // Запуск event loop
    return app.exec();
}
