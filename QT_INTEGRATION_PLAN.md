# Qt Integration Plan - Bronnikov Exercise App

**Дата:** 2026-02-04
**Статус:** Готов к началу (требует установки Qt)

---

## Обзор

План интеграции Qt 6 + QML в существующий C++ проект BronnikovExerciseApp.

### Текущее состояние

- ✅ Консольное приложение полностью функционально (v0.9)
- ✅ C++ backend готов (CapsuleAPI интеграция, база данных, упражнения)
- ✅ Архитектура поддерживает ступени
- ❌ Qt не установлен
- ❌ GUI отсутствует

### Целевое состояние

- ✅ Qt 6.7+ установлен
- ✅ Гибридное приложение: QML UI + C++ backend
- ✅ Консольный режим сохранен (для отладки)
- ✅ Real-time визуализация метрик

---

## Шаг 1: Установка Qt

### macOS (Apple Silicon)

**Вариант A: Homebrew (рекомендуется)**
```bash
# Установка Qt 6
brew install qt@6

# Добавить в PATH (добавить в ~/.zshrc)
echo 'export PATH="/opt/homebrew/opt/qt@6/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc

# Проверка
qmake --version
```

**Вариант B: Qt Online Installer**
1. Скачать с https://www.qt.io/download-qt-installer
2. Установить Qt 6.7+ (выбрать компоненты: Qt Quick, Qt Charts, Qt SVG)
3. Добавить Qt в PATH:
   ```bash
   export PATH="$HOME/Qt/6.7.0/macos/bin:$PATH"
   ```

### Windows

1. Скачать Qt Online Installer
2. Установить Qt 6.7+ для MSVC 2022
3. Добавить в PATH:
   ```
   C:\Qt\6.7.0\msvc2022_64\bin
   ```

### Проверка установки

```bash
# Проверка qmake
qmake --version
# Ожидается: QMake version 3.x, Using Qt version 6.7.x

# Проверка CMake поддержки
cmake --help-module-list | grep Qt
# Должны быть модули Qt6Core, Qt6Gui, Qt6Quick и т.д.
```

---

## Шаг 2: Обновление структуры проекта

### Новая структура директорий

```
BronnikovExerciseApp/
├── CMakeLists.txt                   # ОБНОВИТЬ для Qt
├── include/                         # Существующие C++ заголовки
├── src/
│   ├── main.cpp                     # ОБНОВИТЬ: выбор режима (CLI/GUI)
│   ├── cli/                         # НОВОЕ: консольный режим
│   │   └── CliApp.cpp               # Текущий main.cpp → сюда
│   ├── gui/                         # НОВОЕ: GUI режим
│   │   ├── GuiApp.cpp               # Qt application setup
│   │   ├── models/                  # QML Data Models
│   │   │   ├── ExerciseModel.cpp
│   │   │   ├── MetricsModel.cpp
│   │   │   └── SessionModel.cpp
│   │   └── controllers/             # Business Logic Controllers
│   │       ├── ExerciseController.cpp
│   │       ├── DeviceController.cpp
│   │       └── StatisticsController.cpp
│   ├── capsule/                     # Существующий код (без изменений)
│   ├── exercises/                   # Существующий код (без изменений)
│   └── database/                    # Существующий код (без изменений)
├── qml/                             # НОВОЕ: QML UI файлы
│   ├── main.qml                     # Главное окно
│   ├── screens/
│   │   ├── HomeScreen.qml
│   │   ├── PreparationScreen.qml
│   │   ├── ExerciseScreen.qml
│   │   ├── ResultsScreen.qml
│   │   └── StatisticsScreen.qml
│   ├── components/
│   │   ├── ExerciseCard.qml
│   │   ├── MetricsPanel.qml
│   │   ├── ProgressBar.qml
│   │   ├── StageSelector.qml
│   │   └── RealTimeChart.qml
│   └── styles/
│       ├── Theme.qml                # Цветовая схема
│       ├── Typography.qml           # Шрифты
│       └── Constants.qml            # Константы
└── resources/                       # НОВОЕ: Ресурсы для Qt
    ├── icons/
    ├── images/
    └── fonts/
```

---

## Шаг 3: Обновление CMakeLists.txt

### Основные изменения

```cmake
cmake_minimum_required(VERSION 3.20)
project(BronnikovExerciseApp VERSION 0.10.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# НОВОЕ: Опция для выбора режима сборки
option(BUILD_GUI "Build with Qt GUI" ON)
option(BUILD_CLI "Build CLI version" ON)

# НОВОЕ: Поиск Qt6 (если BUILD_GUI включен)
if(BUILD_GUI)
    find_package(Qt6 6.5 REQUIRED COMPONENTS
        Core
        Gui
        Quick        # QML
        Charts       # Графики
        Svg          # Иконки
    )

    set(CMAKE_AUTOMOC ON)  # Meta-Object Compiler
    set(CMAKE_AUTORCC ON)  # Resource Compiler
    set(CMAKE_AUTOUIC ON)  # UI Compiler
endif()

# Существующие source files
set(COMMON_SOURCES
    src/capsule/CapsuleManager.cpp
    src/capsule/SessionManager.cpp
    src/capsule/MetricsCollector.cpp
    src/exercises/Exercise.cpp
    src/exercises/ExerciseLibrary.cpp
    # ... все остальные существующие файлы
)

# НОВОЕ: GUI sources
if(BUILD_GUI)
    set(GUI_SOURCES
        src/gui/GuiApp.cpp
        src/gui/models/ExerciseModel.cpp
        src/gui/models/MetricsModel.cpp
        src/gui/models/SessionModel.cpp
        src/gui/controllers/ExerciseController.cpp
        src/gui/controllers/DeviceController.cpp
        src/gui/controllers/StatisticsController.cpp
    )

    # QML resources
    qt_add_resources(QML_RESOURCES qml/qml.qrc)
endif()

# НОВОЕ: CLI sources
if(BUILD_CLI)
    set(CLI_SOURCES
        src/cli/CliApp.cpp
    )
endif()

# Основной executable
if(BUILD_GUI)
    add_executable(${PROJECT_NAME}
        src/main.cpp
        ${COMMON_SOURCES}
        ${GUI_SOURCES}
        ${QML_RESOURCES}
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Quick
        Qt6::Charts
        Qt6::Svg
        # Существующие зависимости
        ${CMAKE_SOURCE_DIR}/../CapsuleAPI/Mac/libCapsuleClient.dylib
        SQLite::SQLite3
    )

    # Установка Qt runtime paths
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
    )
endif()

# CLI executable (опционально)
if(BUILD_CLI)
    add_executable(${PROJECT_NAME}_cli
        src/cli/main_cli.cpp
        ${COMMON_SOURCES}
        ${CLI_SOURCES}
    )

    target_link_libraries(${PROJECT_NAME}_cli PRIVATE
        ${CMAKE_SOURCE_DIR}/../CapsuleAPI/Mac/libCapsuleClient.dylib
        SQLite::SQLite3
    )
endif()
```

---

## Шаг 4: Создание QML структуры

### main.qml (базовая структура)

```qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: mainWindow

    width: 1280
    height: 720
    visible: true
    title: "Bronnikov Method - Brain Training"

    // Подключение C++ моделей
    property var exerciseModel: ExerciseModel
    property var metricsModel: MetricsModel
    property var sessionModel: SessionModel

    // Навигация
    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: homeScreen

        // Анимация перехода
        pushEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to: 1
                duration: 300
            }
            PropertyAnimation {
                property: "x"
                from: mainWindow.width
                to: 0
                duration: 300
            }
        }
    }

    // Экраны
    Component {
        id: homeScreen
        HomeScreen {}
    }

    Component {
        id: exerciseScreen
        ExerciseScreen {}
    }

    // ... остальные экраны
}
```

---

## Шаг 5: Создание C++ <-> QML bridge

### ExerciseModel.h

```cpp
#pragma once

#include <QObject>
#include <QAbstractListModel>
#include <QVariant>
#include "ExerciseLibrary.h"

namespace Bronnikov {

class ExerciseModel : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(int currentStage READ currentStage WRITE setCurrentStage NOTIFY currentStageChanged)
    Q_PROPERTY(int exerciseCount READ exerciseCount NOTIFY exercisesChanged)

public:
    enum ExerciseRoles {
        NameRole = Qt::UserRole + 1,
        DescriptionRole,
        DurationRole,
        StageRole,
        OrderRole,
        StageIdRole,
        IconRole
    };

    explicit ExerciseModel(QObject* parent = nullptr);

    // QAbstractListModel interface
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Properties
    int currentStage() const { return m_currentStage; }
    void setCurrentStage(int stage);
    int exerciseCount() const;

    // Invokable methods (можно вызывать из QML)
    Q_INVOKABLE QVariantMap getExercise(int index) const;
    Q_INVOKABLE void selectExercise(int index);

signals:
    void currentStageChanged();
    void exercisesChanged();
    void exerciseSelected(int index);

private:
    std::unique_ptr<ExerciseLibrary> m_library;
    int m_currentStage = 0;
    std::vector<std::shared_ptr<Exercise>> m_currentExercises;

    void updateExerciseList();
};

} // namespace Bronnikov
```

### MetricsModel.h

```cpp
#pragma once

#include <QObject>
#include <QTimer>
#include "MetricsCollector.h"

namespace Bronnikov {

class MetricsModel : public QObject {
    Q_OBJECT

    // Реальные метрики (обновляются каждые 100ms)
    Q_PROPERTY(double alphaPower READ alphaPower NOTIFY metricsUpdated)
    Q_PROPERTY(double betaPower READ betaPower NOTIFY metricsUpdated)
    Q_PROPERTY(double thetaPower READ thetaPower NOTIFY metricsUpdated)
    Q_PROPERTY(double concentration READ concentration NOTIFY metricsUpdated)
    Q_PROPERTY(double relaxation READ relaxation NOTIFY metricsUpdated)
    Q_PROPERTY(double heartRate READ heartRate NOTIFY metricsUpdated)
    Q_PROPERTY(double successRate READ successRate NOTIFY metricsUpdated)

    // История для графиков
    Q_PROPERTY(QVariantList alphaHistory READ alphaHistory NOTIFY historyUpdated)
    Q_PROPERTY(QVariantList betaHistory READ betaHistory NOTIFY historyUpdated)
    Q_PROPERTY(QVariantList thetaHistory READ thetaHistory NOTIFY historyUpdated)

public:
    explicit MetricsModel(QObject* parent = nullptr);

    void setMetricsCollector(MetricsCollector* collector);

    // Getters
    double alphaPower() const { return m_currentMetrics.alpha_power; }
    double betaPower() const { return m_currentMetrics.beta_power; }
    double thetaPower() const { return m_currentMetrics.theta_power; }
    double concentration() const { return m_currentMetrics.concentration; }
    double relaxation() const { return m_currentMetrics.relaxation; }
    double heartRate() const { return m_currentMetrics.heart_rate; }
    double successRate() const { return m_successRate; }

    QVariantList alphaHistory() const;
    QVariantList betaHistory() const;
    QVariantList thetaHistory() const;

    Q_INVOKABLE void startMonitoring();
    Q_INVOKABLE void stopMonitoring();
    Q_INVOKABLE void clearHistory();

signals:
    void metricsUpdated();
    void historyUpdated();

private slots:
    void updateMetrics();

private:
    MetricsCollector* m_collector = nullptr;
    MetricsSnapshot m_currentMetrics;
    double m_successRate = 0.0;

    QTimer* m_updateTimer = nullptr;

    // История последних 120 секунд (по 1 точке в секунду)
    std::vector<double> m_alphaHistory;
    std::vector<double> m_betaHistory;
    std::vector<double> m_thetaHistory;
    static constexpr size_t MAX_HISTORY_SIZE = 120;
};

} // namespace Bronnikov
```

---

## Шаг 6: GuiApp.cpp - точка входа

```cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "models/ExerciseModel.h"
#include "models/MetricsModel.h"
#include "models/SessionModel.h"

int gui_main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    // Настройка приложения
    app.setOrganizationName("Bronnikov Method");
    app.setOrganizationDomain("bronnikov.com");
    app.setApplicationName("Brain Training App");

    // Создание моделей
    Bronnikov::ExerciseModel exerciseModel;
    Bronnikov::MetricsModel metricsModel;
    Bronnikov::SessionModel sessionModel;

    // Создание QML engine
    QQmlApplicationEngine engine;

    // Регистрация моделей в QML context
    engine.rootContext()->setContextProperty("ExerciseModel", &exerciseModel);
    engine.rootContext()->setContextProperty("MetricsModel", &metricsModel);
    engine.rootContext()->setContextProperty("SessionModel", &sessionModel);

    // Загрузка главного QML файла
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    engine.load(url);

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
```

---

## Шаг 7: Обновление main.cpp

```cpp
#include <iostream>
#include <string>

// Forward declarations
int cli_main(int argc, char* argv[]);
int gui_main(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    // Проверка флагов запуска
    bool force_cli = false;
    bool force_gui = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--cli") force_cli = true;
        if (arg == "--gui") force_gui = true;
        if (arg == "--help" || arg == "-h") {
            std::cout << "Bronnikov Exercise App v0.10\n\n";
            std::cout << "Usage:\n";
            std::cout << "  BronnikovExerciseApp          Start GUI (default)\n";
            std::cout << "  BronnikovExerciseApp --cli    Start CLI version\n";
            std::cout << "  BronnikovExerciseApp --gui    Force GUI version\n";
            std::cout << "  BronnikovExerciseApp --help   Show this help\n";
            return 0;
        }
    }

    // Выбор режима
    if (force_cli || (!force_gui && std::getenv("BRONNIKOV_CLI"))) {
        std::cout << "Starting in CLI mode...\n";
        return cli_main(argc, argv);
    } else {
        // По умолчанию GUI
        return gui_main(argc, argv);
    }
}
```

---

## Шаг 8: План поэтапной разработки

### Неделя 1: Инфраструктура
- [x] Установка Qt 6.7+
- [ ] Обновление CMakeLists.txt
- [ ] Создание базовой структуры QML
- [ ] Настройка сборки (GUI + CLI)
- [ ] Проверка работоспособности (пустое окно Qt)

### Неделя 2: Home Screen
- [ ] ExerciseModel + QML интеграция
- [ ] HomeScreen.qml (список упражнений)
- [ ] ExerciseCard компонент
- [ ] StageSelector компонент
- [ ] Навигация к экрану упражнения

### Неделя 3: Exercise Screen (часть 1)
- [ ] PreparationScreen.qml (подключение устройства)
- [ ] ExerciseScreen.qml (базовая структура)
- [ ] MetricsPanel компонент
- [ ] Timer и progress indicators

### Неделя 4: Exercise Screen (часть 2)
- [ ] MetricsModel + real-time обновления
- [ ] RealTimeChart компонент (QtCharts)
- [ ] Instructions panel
- [ ] Pause/Resume/Stop функциональность

### Неделя 5: Results & Statistics
- [ ] ResultsScreen.qml
- [ ] SessionModel integration
- [ ] StatisticsScreen.qml
- [ ] Charts для трендов
- [ ] Calendar widget

### Неделя 6: Полировка
- [ ] Анимации и transitions
- [ ] Темная тема
- [ ] Settings screen
- [ ] Иконки и graphics
- [ ] Тестирование и bug fixes

---

## Шаг 9: Тестовый чеклист

### После установки Qt
- [ ] `qmake --version` показывает Qt 6.7+
- [ ] CMake находит Qt модули
- [ ] Успешная компиляция с Qt

### После базовой интеграции
- [ ] Приложение запускается с пустым окном
- [ ] CLI режим все еще работает (`--cli` флаг)
- [ ] Можно переключаться между режимами

### После Home Screen
- [ ] Отображается список упражнений
- [ ] Можно выбрать ступень
- [ ] Клик по карточке переходит к упражнению

### После Exercise Screen
- [ ] Устройство подключается
- [ ] Метрики обновляются в real-time
- [ ] График рисуется корректно
- [ ] Таймер работает

### После Results
- [ ] Данные сохраняются в БД
- [ ] Статистика отображается
- [ ] Графики трендов работают

---

## Потенциальные проблемы и решения

### Проблема 1: Qt не находится CMake

**Решение:**
```bash
# Указать путь к Qt явно
cmake -DCMAKE_PREFIX_PATH=/opt/homebrew/opt/qt@6 ..
```

### Проблема 2: Real-time updates медленные

**Решение:**
- Использовать QTimer с интервалом 100ms
- Обновлять только измененные property
- Throttling для графиков (обновление раз в секунду)

### Проблема 3: QtCharts не установлен

**Решение:**
```bash
brew install qt6-charts
# или через Qt Maintenance Tool
```

### Проблема 4: QML не находит компоненты

**Решение:**
- Проверить qml.qrc файл
- Убедиться что путь начинается с `qrc:/`
- Пересобрать проект (clean + rebuild)

---

## Ресурсы для изучения

### Документация
- [Qt 6 Documentation](https://doc.qt.io/qt-6/)
- [QML Tutorial](https://doc.qt.io/qt-6/qmltutorial.html)
- [QtCharts Overview](https://doc.qt.io/qt-6/qtcharts-index.html)

### Примеры
- Qt Examples: `$QTDIR/examples/quick/`
- QtCharts Examples: `$QTDIR/examples/charts/`

### Инструменты
- Qt Creator IDE (рекомендуется для QML)
- Qt Design Studio (для дизайна)
- qmlscene (для тестирования QML файлов)

---

## Следующий шаг

**Установите Qt 6.7+** используя инструкции из Шага 1, затем приступайте к Шагу 2 (обновление структуры проекта).

После установки запустите:
```bash
cd BronnikovExerciseApp
git checkout -b feature/qt-gui
# Начинайте реализацию по плану
```

