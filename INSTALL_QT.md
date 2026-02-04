# Инструкция по установке Qt 6 для GUI версии

**Дата:** 2026-02-04
**Версия Qt:** 6.7+ (рекомендуется 6.8)
**Цель:** Установить Qt для сборки графического интерфейса BronnikovExerciseApp

---

## Зачем нужна установка Qt?

Графическая версия приложения использует **Qt 6** и **QML** для создания современного UI с:
- Real-time графиками мозговой активности
- Интерактивными карточками упражнений
- Плавными анимациями и переходами
- Адаптивным дизайном (1280px - 4K)

Консольная версия продолжит работать без Qt.

---

## macOS - Установка через Homebrew (Рекомендуется)

### Вариант 1: Homebrew (Быстро, 10-15 минут)

#### Шаг 1: Установка Qt
```bash
# Установка Qt 6
brew install qt@6

# Добавление Qt в PATH
echo 'export PATH="/opt/homebrew/opt/qt@6/bin:$PATH"' >> ~/.zshrc
echo 'export Qt6_DIR="/opt/homebrew/opt/qt@6/lib/cmake/Qt6"' >> ~/.zshrc
source ~/.zshrc
```

#### Шаг 2: Проверка установки
```bash
# Проверка версии qmake
qmake --version
# Ожидается: QMake version 3.1, Using Qt version 6.x.x

# Проверка версии Qt
ls /opt/homebrew/opt/qt@6/lib/cmake/Qt6
# Должны быть директории: Qt6Core, Qt6Gui, Qt6Quick, Qt6Qml, Qt6Charts, Qt6Svg
```

#### Шаг 3: Проверка компонентов
```bash
# Проверка наличия QtCharts (для графиков)
ls /opt/homebrew/opt/qt@6/lib/cmake/ | grep -i charts
# Ожидается: Qt6Charts

# Проверка наличия QtQuick (для QML)
ls /opt/homebrew/opt/qt@6/lib/cmake/ | grep -i quick
# Ожидается: Qt6Quick, Qt6QuickControls2
```

**Если QtCharts отсутствует:**
```bash
# Homebrew версия Qt может не включать QtCharts
# В этом случае используйте Online Installer (Вариант 2)
```

---

### Вариант 2: Qt Online Installer (Полная установка, 1-2 часа)

Используйте этот вариант если:
- Homebrew версия не содержит QtCharts
- Нужна последняя версия Qt
- Требуется Qt Creator IDE

#### Шаг 1: Скачивание установщика
1. Перейти на https://www.qt.io/download-qt-installer
2. Скачать **Qt Online Installer** для macOS
3. Открыть `.dmg` файл и запустить установщик

#### Шаг 2: Регистрация (опционально)
- Можно создать бесплатный Qt Account
- Или выбрать "Skip" для использования open-source версии

#### Шаг 3: Выбор компонентов

**ВАЖНО:** Установите следующие компоненты:

**Qt 6.8.0 (или новее):**
- ✅ macOS
- ✅ Qt Quick (QML)
- ✅ Qt Quick Controls
- ✅ Qt Charts
- ✅ Qt SVG
- ✅ Sources (опционально, для отладки)

**Qt Creator (опционально):**
- ✅ Qt Creator 14.x - для редактирования QML

**Дополнительные инструменты:**
- ✅ CMake (если не установлен через Homebrew)

#### Шаг 4: Путь установки
Рекомендуемый путь: `/Users/YOUR_USERNAME/Qt`

#### Шаг 5: Добавление Qt в PATH
```bash
# Замените 6.8.0 на вашу версию
echo 'export PATH="/Users/$(whoami)/Qt/6.8.0/macos/bin:$PATH"' >> ~/.zshrc
echo 'export Qt6_DIR="/Users/$(whoami)/Qt/6.8.0/macos/lib/cmake/Qt6"' >> ~/.zshrc
source ~/.zshrc
```

#### Шаг 6: Проверка установки
```bash
qmake --version
# Ожидается: QMake version 3.1, Using Qt version 6.8.0
```

---

## Windows - Установка через Qt Online Installer

### Системные требования
- Windows 10/11 (64-bit)
- Visual Studio 2019 или 2022 (Community Edition достаточно)
- Минимум 10 GB свободного места

### Шаг 1: Установка Visual Studio (если не установлена)
1. Скачать Visual Studio Community 2022: https://visualstudio.microsoft.com/downloads/
2. В установщике выбрать **"Desktop development with C++"**
3. Установить

### Шаг 2: Скачивание Qt Online Installer
1. Перейти на https://www.qt.io/download-qt-installer
2. Скачать **Qt Online Installer** для Windows
3. Запустить `qt-unified-windows-x64-online.exe`

### Шаг 3: Выбор компонентов

**Qt 6.8.0 (или новее):**
- ✅ MSVC 2022 64-bit
- ✅ Qt Quick (QML)
- ✅ Qt Quick Controls
- ✅ Qt Charts
- ✅ Qt SVG
- ✅ Sources (опционально)

**Qt Creator:**
- ✅ Qt Creator 14.x

**Дополнительные инструменты:**
- ✅ CMake
- ✅ Ninja

### Шаг 4: Путь установки
Рекомендуемый путь: `C:\Qt`

### Шаг 5: Добавление Qt в PATH

**PowerShell (запустить от имени администратора):**
```powershell
# Замените 6.8.0 на вашу версию
$qtPath = "C:\Qt\6.8.0\msvc2022_64\bin"
[Environment]::SetEnvironmentVariable("Path", $env:Path + ";$qtPath", [EnvironmentVariableTarget]::Machine)

# Добавить Qt6_DIR
[Environment]::SetEnvironmentVariable("Qt6_DIR", "C:\Qt\6.8.0\msvc2022_64\lib\cmake\Qt6", [EnvironmentVariableTarget]::Machine)
```

### Шаг 6: Перезагрузка
Перезагрузите Windows для применения PATH

### Шаг 7: Проверка установки
```powershell
qmake --version
# Ожидается: QMake version 3.1, Using Qt version 6.8.0
```

---

## После установки Qt

### 1. Применение CMakeLists_GUI.txt

```bash
cd BronnikovExerciseApp

# Замена стандартного CMakeLists.txt на версию с Qt
cp CMakeLists_GUI.txt CMakeLists.txt

# Или создание резервной копии
mv CMakeLists.txt CMakeLists_CLI.txt
cp CMakeLists_GUI.txt CMakeLists.txt
```

### 2. Первая сборка GUI версии

**macOS:**
```bash
# Удалить старую build директорию
rm -rf build

# Создать новую сборку с Qt
cmake -S . -B ./build -G "Xcode" -DBUILD_GUI=ON -DBUILD_CLI=ON
cmake --build ./build --config Release

# Запуск GUI версии
./build/Release/BronnikovExerciseApp

# Запуск CLI версии (если нужна)
./build/Release/BronnikovExerciseAppCLI
```

**Windows:**
```powershell
# Удалить старую build директорию
Remove-Item -Recurse -Force build

# Создать новую сборку с Qt
cmake -S . -B ./build -G "Visual Studio 17 2022" -A x64 -DBUILD_GUI=ON -DBUILD_CLI=ON
cmake --build ./build --config Release

# Запуск GUI версии
.\build\Release\BronnikovExerciseApp.exe

# Запуск CLI версии (если нужна)
.\build\Release\BronnikovExerciseAppCLI.exe
```

### 3. Ожидаемый результат

При успешной сборке вы должны увидеть:
```
-- Qt6 found: /opt/homebrew/opt/qt@6/lib/cmake/Qt6
-- Building GUI version
-- Qt6 Version: 6.8.0
==============================================
Bronnikov Exercise App Configuration Summary
==============================================
Version: 0.10.0
Platform: Mac
C++ Standard: C++20
Build GUI: ON
Build CLI: ON
Qt6 Version: 6.8.0
CapsuleAPI Library: .../libCapsuleClient.dylib
==============================================
[100%] Built target BronnikovExerciseApp
```

---

## Решение проблем

### Проблема: Qt не найден CMake

**Симптом:**
```
CMake Error: Could not find a package configuration file provided by "Qt6"
```

**Решение (macOS Homebrew):**
```bash
# Явно указать путь к Qt
export Qt6_DIR="/opt/homebrew/opt/qt@6/lib/cmake/Qt6"
cmake -S . -B ./build -G "Xcode" -DBUILD_GUI=ON
```

**Решение (macOS Qt Installer):**
```bash
# Замените 6.8.0 на вашу версию
export Qt6_DIR="/Users/$(whoami)/Qt/6.8.0/macos/lib/cmake/Qt6"
cmake -S . -B ./build -G "Xcode" -DBUILD_GUI=ON
```

**Решение (Windows):**
```powershell
# В CMake GUI или командной строке
cmake -S . -B ./build -G "Visual Studio 17 2022" -DQt6_DIR="C:\Qt\6.8.0\msvc2022_64\lib\cmake\Qt6"
```

---

### Проблема: QtCharts не найден

**Симптом:**
```
Could not find a package configuration file provided by "Qt6Charts"
```

**Решение:**
QtCharts не установлен. Используйте Qt Online Installer вместо Homebrew и убедитесь, что выбрали **Qt Charts** в компонентах.

---

### Проблема: qmake не найден в PATH

**Симптом:**
```bash
qmake --version
# zsh: command not found: qmake
```

**Решение (macOS):**
```bash
# Проверить установлен ли Qt
ls /opt/homebrew/opt/qt@6/bin/qmake

# Если файл существует, добавить в PATH
echo 'export PATH="/opt/homebrew/opt/qt@6/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc
```

**Решение (Windows):**
- Проверить PATH в "Переменные среды"
- Добавить `C:\Qt\6.8.0\msvc2022_64\bin` вручную
- Перезагрузить систему

---

### Проблема: Не запускается GUI приложение (macOS)

**Симптом:**
```
dyld: Library not loaded: @rpath/QtCore.framework/Versions/A/QtCore
```

**Решение:**
Qt библиотеки не найдены. Убедитесь что:
1. Qt установлен корректно
2. PATH настроен
3. Пересоберите проект с чистой build директорией

```bash
rm -rf build
cmake -S . -B ./build -G "Xcode" -DBUILD_GUI=ON
cmake --build ./build --config Release
```

---

## Использование Qt Creator (опционально)

Qt Creator - удобная IDE для разработки QML UI.

### Открытие проекта в Qt Creator

1. Запустить Qt Creator
2. File → Open File or Project
3. Выбрать `BronnikovExerciseApp/CMakeLists.txt`
4. В настройках сборки выбрать:
   - Kit: Desktop Qt 6.x.x
   - Build directory: `build`
   - CMake arguments: `-DBUILD_GUI=ON`
5. Нажать "Configure Project"

### Редактирование QML

Qt Creator предоставляет:
- Syntax highlighting для QML
- Auto-completion
- Live preview (Qt Quick Designer)
- QML Profiler для оптимизации

---

## Тестирование установки Qt

Для проверки что Qt работает корректно, создайте простой тест:

**test_qt.cpp:**
```cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qDebug() << "Qt version:" << QT_VERSION_STR;
    qDebug() << "Qt Charts available:" << QT_CONFIG(charts);
    return 0;
}
```

**CMakeLists.txt:**
```cmake
cmake_minimum_required(VERSION 3.20)
project(QtTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Charts)

add_executable(QtTest test_qt.cpp)
target_link_libraries(QtTest Qt6::Core Qt6::Charts)
```

**Сборка:**
```bash
cmake -S . -B build
cmake --build build
./build/QtTest
```

**Ожидаемый вывод:**
```
Qt version: 6.8.0
Qt Charts available: true
```

---

## Дальнейшие шаги

После успешной установки Qt:

1. ✅ Qt установлен и PATH настроен
2. ✅ CMakeLists.txt заменен на CMakeLists_GUI.txt
3. ✅ Первая сборка успешна
4. ⏳ Реализация C++ моделей (ExerciseModel.cpp, MetricsModel.cpp, SessionModel.cpp)
5. ⏳ Интеграция C++ <-> QML
6. ⏳ QtCharts интеграция для графиков
7. ⏳ Device integration
8. ⏳ Тестирование GUI

Следующий документ для изучения: **QT_INTEGRATION_PLAN.md** - детальный план интеграции Qt в проект.

---

**Удачи в установке Qt! 🚀**
