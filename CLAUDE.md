# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

BronnikovExerciseApp is a desktop application for practicing Bronnikov Method exercises with real-time EEG monitoring using Neiry brain-computer interface devices. It integrates with CapsuleAPI (C library) to collect neurofeedback metrics and provides both CLI and GUI versions.

**Key Technologies:**
- C++20 with Qt6/QML for GUI
- CapsuleAPI for Neiry device integration
- SQLite for session storage
- CMake build system

## Build Commands

### macOS

Standard build:
```bash
cmake -S . -B ./build -G "Xcode"
cmake --build ./build --config Release
```

For Apple Silicon (M1/M2/M3), run in x86_64 mode:
```bash
arch -x86_64 zsh
cmake -S . -B ./build -G "Xcode"
cmake --build ./build --config Release
```

### Windows

```bash
cmake -S . -B ./build -G "Visual Studio 17 2022" -A x64
cmake --build ./build --config Release
```

### Run the Application

**macOS:**
```bash
./build/Release/BronnikovExerciseApp      # GUI version
./build/Release/BronnikovExerciseAppCLI   # CLI version
```

**Windows:**
```bash
build\Release\BronnikovExerciseApp.exe    # GUI version
build\Release\BronnikovExerciseAppCLI.exe # CLI version
```

### Build Options

- `BUILD_GUI=ON/OFF` - Enable/disable GUI version (default: ON)
- `BUILD_CLI=ON/OFF` - Enable/disable CLI version (default: ON)

Example:
```bash
cmake -S . -B ./build -G "Xcode" -DBUILD_GUI=OFF -DBUILD_CLI=ON
```

## Architecture Overview

### Layered Design

The application follows a clear layered architecture:

```
┌─────────────────────────────────────┐
│  UI Layer (Qt/QML or Console)       │  main_gui.cpp / main_cli.cpp
├─────────────────────────────────────┤
│  Controllers (Qt Models)            │  DeviceController, ExerciseController
├─────────────────────────────────────┤
│  Business Logic                     │  Exercise, ExerciseLibrary, SessionDatabase
├─────────────────────────────────────┤
│  Integration Layer                  │  CapsuleManager, SessionManager, MetricsCollector
├─────────────────────────────────────┤
│  CapsuleAPI (C library)             │  libCapsuleClient.dylib / CapsuleClient.dll
└─────────────────────────────────────┘
```

### Critical Architectural Patterns

**1. Exercise Hierarchy System**

All exercises inherit from base `Exercise` class and are organized by stages:

- **Preparatory Stage** (Stage 0): 10 exercises - basic bioenergy sensitivity
- **Stage 1** "Экология духа": 18 exercises - from official 2011 methodology
- **Stage 2** "Зрение вне глаз": 25 exercises - alternative vision
- **Stage 3** "Экран ЛБК": 15 exercises - biocomputer work
- **Stage 4** "Радарное видение": 10 exercises - information field work

Each exercise defines:
- `m_stage` (ExerciseStage enum)
- `m_order_in_stage` (1, 2, 3...)
- `initializeInstructions()` - step-by-step guidance
- `initializeTargetMetrics()` - expected brain state (alpha/theta/concentration changes)

**2. Three-Phase Metrics Collection**

The Bronnikov methodology requires comparing metrics across 3 phases:

1. **Baseline** (30-60 sec) - resting state before exercise
2. **Active** - main practice with instructions
3. **Post** (30-60 sec) - recovery state after exercise

Success rate is calculated by comparing phase changes against target metrics using weighted scoring algorithm.

**3. Asynchronous CapsuleAPI Integration**

CapsuleAPI is event-driven and single-threaded:
- Must call `clCClient_Update()` regularly (every 50ms) in main loop
- All operations (connect, start session, calibration) are async with callbacks
- Callbacks don't support `user_data`, so global maps link handles to C++ objects:
  ```cpp
  static std::map<clCNFBMetricProductivity, MetricsCollector*> g_productivity_map;
  ```

**4. Qt/QML Integration via Controllers**

Qt Controllers expose C++ backend to QML:
- **DeviceController**: Device connection, session management, calibration
- **ExerciseController**: Exercise lifecycle, instruction flow
- **MetricsModel**: Real-time metrics updates for UI
- **ExerciseModel**: Exercise library access

Controllers use `Q_PROPERTY` and `Q_INVOKABLE` for QML binding.

## Key Components

### CapsuleManager (src/capsule/CapsuleManager.cpp)

Manages connection to CapsuleAPI backend and device discovery. Handles:
- Backend connection (`inproc://capsule` for embedded mode)
- Device locator for Bluetooth scanning
- Device lifecycle (connect/disconnect)
- Firmware and battery monitoring

### SessionManager (src/capsule/SessionManager.cpp)

Controls EEG session lifecycle:
- Session creation/destruction
- Device mode switching (Signal, PPG, MEMS, Idle)
- NFB calibration (Individual Alpha Frequency detection)
- Session state monitoring (Starting, Active, Pausing, Stopping)

### MetricsCollector (src/capsule/MetricsCollector.cpp)

Initializes and collects data from CapsuleAPI classifiers:
- **CNFB**: Alpha/Beta/Theta power extraction
- **CNFBMetricsProductivity**: Concentration, Relaxation, Fatigue, Gravity
- **CEmotions**: Focus, Chill, Stress, Anger, Self-Control
- **CCardio**: Heart rate, stress index from PPG sensor

Metrics are accumulated into `MetricsSnapshot` structs for each phase.

### Exercise Base Class (src/exercises/Exercise.cpp)

Provides core functionality:
- Three-phase metrics collection (`startBaseline()`, `startActive()`, `startPost()`)
- Instruction sequencing with timing
- Phase comparison and success rate calculation
- Target state evaluation with weighted scoring

### ExerciseLibrary (src/exercises/ExerciseLibrary.cpp)

Registry of all 78 exercises with:
- Grouping by stage (`getExercisesByStage()`)
- Menu generation with stage hierarchy
- Quick access by stage and order (`getExerciseByStageOrder()`)

### SessionDatabase (src/database/SessionDatabase.cpp)

SQLite storage for:
- Session history with all three phases
- User profiles with baseline persistence
- Stage progress tracking (completion percentage)
- Extended schema stores: baseline metrics, post metrics, changes, effectiveness

## Important Implementation Details

### Adding New Exercises

1. Create header and implementation files (e.g., `NewExercise.h/cpp`)
2. Inherit from `Exercise` base class
3. Set `m_stage` and `m_order_in_stage` in constructor
4. Override:
   - `initializeInstructions()` - define step-by-step guidance with durations
   - `initializeTargetMetrics()` - set expected brain state changes (alpha/theta/concentration)
5. Add to `CMakeLists.txt` in `EXERCISE_SOURCES` under appropriate stage section
6. Register in `ExerciseLibrary::registerExercises()`

See `src/exercises/MorphologicalFields.cpp` as reference implementation.

### Stage Organization Rules

**CRITICAL:** Exercise numbering follows strict methodology structure:
- Preparatory: 0.1 - 0.10
- Stage 1: 1.1 - 1.18 (from official 2011 methodology "Экология духа")
- Stage 2: 2.1 - 2.25 (alternative vision exercises)
- Stage 3: 3.1 - 3.15 (biocomputer work)
- Stage 4: 4.1 - 4.10 (information field work)

When modifying stage assignments, update:
1. `m_stage` value in exercise constructor
2. CMakeLists.txt comments for correct stage section
3. ExerciseLibrary if stage counts change

### CapsuleAPI Integration Gotchas

**String handling:**
- API accepts `const char*` directly (NOT `clCString_New()`)
- Use `clCString_CStr()` to extract strings (NOT `clCString_GetCStr()`)

**Session management:**
- Create via `clCClient_CreateSession()` (NOT `clCSession_Create()`)
- Sessions are auto-managed, no explicit destroy needed
- State checking: use `clCSessionState` enum values exactly

**Classifier initialization:**
```cpp
// Correct
clCNFBMetricsProductivity_Create(session, path, speed, maxSpeed, slowDown, &error);
clCEmotions_Create(session, speed, maxSpeed);  // Requires 2 speed params

// Incorrect
clCNFBMetricsProductivity_Create(session);  // Missing required params
```

### Platform-Specific Paths

CapsuleAPI library paths are auto-detected by platform:
- **macOS**: `../CapsuleAPI/Mac/libCapsuleClient.dylib`
- **Windows**: `../CapsuleAPI/Win/build/Release/CapsuleClient.dll`

Library is copied to output directory post-build. First run may require permissions approval on macOS (normal behavior for unsigned dylib).

## Testing

No automated tests currently. Manual testing workflow:
1. Connect Neiry device (Band/Buds/Headphones)
2. Ensure battery > 20%
3. Run application
4. Verify device discovery and connection
5. Start exercise and monitor real-time metrics
6. Check CSV export in `data/` directory
7. Verify session saved in `data/bronnikov_sessions.db`

## Common Development Patterns

### Working with QML

QML files in `qml/` directory are compiled into application via `qml.qrc` resource file. When modifying QML:
1. Changes are hot-reloadable in debug mode
2. Resource file automatically includes new QML files
3. Use `Theme.qml` for consistent styling
4. Custom components go in `qml/components/`

### Database Schema Updates

When extending `SessionDatabase`:
1. Update struct definitions in `SessionDatabase.h`
2. Modify SQL schema in `createTables()`
3. Add migration logic in `open()` or dedicated migration method
4. Update `saveSession()` and query methods

### Metrics Flow Debugging

Metrics flow through multiple layers. Debug in this order:
1. CapsuleAPI callbacks firing? (add logging in MetricsCollector)
2. MetricsSnapshot populated? (check `getCurrentMetrics()`)
3. Exercise receiving updates? (verify `update()` called)
4. UI displaying values? (check Qt Model properties)

## File Organization

```
include/          - All header files (Exercise.h, CapsuleManager.h, etc.)
src/
  capsule/        - CapsuleAPI integration layer
  exercises/      - 78 exercise implementations + ExerciseLibrary
  database/       - SQLite storage (SessionDatabase, UserProfile)
  gui/
    controllers/  - Qt controllers for QML (DeviceController, ExerciseController)
    models/       - Qt models (MetricsModel, ExerciseModel, SessionModel)
    main_gui.cpp  - GUI application entry point
  cli/
    main_cli.cpp  - Console application entry point
qml/              - Qt/QML UI files
  components/     - Reusable QML components
  screens/        - Main application screens
  styles/         - Theme and styling
data/             - Runtime data (created automatically)
  bronnikov_sessions.db  - SQLite database
  session_*.csv          - Detailed metrics exports
```

## Resources

- **CapsuleAPI Documentation**: `../CapsuleAPI/Docs/html/index.html` (Doxygen)
- **Bronnikov Method**: Exercises based on official 2011 methodology "Экология духа"
- **Project Roadmap**: `ROADMAP.md` - full development history and future plans
- **Build Notes**: `BUILD_SUCCESS.md` and `BUILD_SUCCESS_GUI.md` - platform-specific build instructions
