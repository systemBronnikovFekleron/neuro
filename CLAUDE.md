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

### Prerequisites

**Qt 6.7+ Required for GUI:**

The GUI version requires Qt 6.7 or later. Install Qt using one of these methods:

**macOS:**
```bash
# Option 1: Homebrew (easiest)
brew install qt@6

# Option 2: Qt Online Installer
# Download from https://www.qt.io/download-qt-installer
# Install Qt 6.8.0 with macOS components
```

**Windows:**
- Download Qt Online Installer from https://www.qt.io/download-qt-installer
- Install Qt 6.8.0 with MSVC 2022 kit

**Set Qt6_DIR environment variable:**
```bash
# If using Homebrew (macOS)
export Qt6_DIR="/opt/homebrew/opt/qt@6/lib/cmake/Qt6"

# If using Qt Installer (macOS)
export Qt6_DIR="$HOME/Qt/6.8.0/macos/lib/cmake/Qt6"

# If using Qt Installer (Windows)
set Qt6_DIR=C:\Qt\6.8.0\msvc2022_64\lib\cmake\Qt6
```

**Build without GUI:**
If you don't need the GUI version, build CLI-only:
```bash
cmake -S . -B ./build -DBUILD_GUI=OFF -DBUILD_CLI=ON
```

### macOS

Standard build (with GUI):
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

### Testing and Development Scripts

**Quick testing with automatic logging:**
```bash
./run_test.sh  # Launch app with timestamped log file
```

**Device connection testing:**
```bash
./test_device.sh  # Verify Neiry device discovery and connection
```

**Settings and database verification:**
```bash
./test_settings.sh  # Check database schema and user profiles
```

**Monitor logs in real-time:**
```bash
./monitor_logs.sh  # Tail latest log file with filtering
```

These scripts are located in the project root and are useful for rapid development iteration.

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

All exercises inherit from base `Exercise` class and are organized by stages. **CRITICAL:** Version 1.1 introduced major reorganization to match official Bronnikov Method structure.

**Complete Exercise Structure (78 exercises total):**

| Stage | Name | Exercises | Numbering | Description |
|-------|------|-----------|-----------|-------------|
| Preparatory | Базовая энергетика | 10 | 0.1 - 0.10 | Basic bioenergy sensitivity and body awareness |
| Stage 1 | "Экология духа" | 18 | 1.1 - 1.18 | Energy work, emotional balance, spirit ecology |
| Stage 2 | "Зрение вне глаз" | 25 | 2.1 - 2.25 | Alternative vision development, bio-screen work |
| Stage 3 | "Экран ЛБК" | 15 | 3.1 - 3.15 | Biocomputer screen, information processing |
| Stage 4 | "Радарное видение" | 10 | 4.1 - 4.10 | Radar vision, information field perception |

**Source:** Official 2011 methodology document `Metodichka_2-3_Stupeni.doc` located in `/Users/avdemkin/code_vibe/Neuro/Docs_SBF/`

**Implementation details:**

Each exercise defines:
- `m_stage` (ExerciseStage enum: Preparatory, Stage1, Stage2, Stage3, Stage4)
- `m_order_in_stage` (1, 2, 3... within that stage)
- `initializeInstructions()` - step-by-step guidance with durations
- `initializeTargetMetrics()` - expected brain state (alpha/theta/concentration changes)

**Progression rules:**
- Users must complete Preparatory stage before Stage 1
- Each stage builds on previous stages
- Exercise difficulty increases within each stage
- Some exercises have prerequisites (e.g., Stage 2.5 requires 2.1-2.4 completion)

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

### Database Schema: Three-Phase Metrics Storage

**Version 0.9+ Extended Schema:**

The database stores metrics from all three phases of each session to enable phase comparison analysis:

**Sessions table structure (21 new fields added):**

1. **Baseline Phase (7 fields):**
   - `baseline_concentration`, `baseline_relaxation`, `baseline_fatigue`
   - `baseline_alpha`, `baseline_beta`, `baseline_theta`
   - `baseline_heart_rate`

2. **Active Phase (original 7 fields):**
   - `concentration`, `relaxation`, `fatigue`
   - `alpha_power`, `beta_power`, `theta_power`
   - `heart_rate`

3. **Post Phase (7 fields):**
   - `post_concentration`, `post_relaxation`, `post_fatigue`
   - `post_alpha`, `post_beta`, `post_theta`
   - `post_heart_rate`

4. **Calculated Changes (6 fields):**
   - `concentration_change`, `relaxation_change`, `fatigue_change`
   - `alpha_change`, `beta_change`, `theta_change`

5. **Effectiveness Metrics (2 fields):**
   - `target_achievement` - how well active phase matched targets (0-100%)
   - `recovery_quality` - how well post phase returned to baseline (0-100%)

**Usage:**

```cpp
// Exercise automatically collects all phases
exercise.startBaseline();   // Records baseline metrics
exercise.startActive();     // Records active metrics
exercise.startPost();       // Records post metrics

// Calculate changes
SessionRecord record = exercise.comparePhases();

// Save to database (all 21+ fields)
sessionDatabase.saveSession(record);
```

**Migration from older versions:**
Older sessions without baseline/post data will have NULL values in new fields. The schema is backward compatible.

### Adaptive Exercise Duration

Exercise duration automatically adapts to user's practice level for optimal training:

**Duration Ranges by Practice Level:**

| Practice Level | Duration Range | Default | Use Case |
|----------------|----------------|---------|----------|
| Beginner (0) | 1-2 minutes | 1 min | New practitioners, learning basics |
| Intermediate (1) | 2-5 minutes | 2 min | Regular practice, building stamina |
| Expert (2) | 5-10 minutes | 5 min | Advanced practitioners, deep states |
| Manual | 1-20 minutes | User choice | Custom duration override |

**Implementation:**

```cpp
// Set recommended duration based on user level
exercise.setRecommendedDuration(PracticeLevel::Beginner, false);  // min duration
exercise.setRecommendedDuration(PracticeLevel::Intermediate, true);  // max duration

// Manual override (ignores practice level)
exercise.setDuration(15);  // 15 minutes

// Get current duration
int duration = exercise.getDuration();  // in minutes
```

**QML Integration (PreparationScreen.qml):**

The preparation screen provides UI for duration selection:
- Radio buttons: "Рекомендуемая" (Auto) vs "Вручную" (Manual)
- SpinBox for manual selection (1-20 minutes range)
- Duration automatically set via `ExerciseController.setDuration()`
- Practice level loaded from user profile: `sessionModel.practiceLevel`

**User Profile:**
- `practice_level` stored in SQLite `users` table (0=Beginner, 1=Intermediate, 2=Expert)
- Loaded via `SessionModel.loadUserProfile()`
- Accessible in QML as property: `sessionModel.practiceLevel`
- Can be updated in Settings screen

**Best Practices:**
- Beginners should start with 1-2 min to avoid fatigue
- Increase duration gradually as comfort improves
- Monitor fatigue metrics during longer sessions
- Manual override useful for specific exercises requiring longer practice

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

### Demo Mode (Testing Without Device)

The application includes a Demo Mode for development and testing without physical Neiry hardware:

**How to Enable Demo Mode:**
1. Launch the application
2. Go to Settings (⚙️ Настройки)
3. Enable "Использовать демо-режим" toggle
4. Return to main screen

**What Demo Mode Provides:**
- **Random but realistic metrics** - Generates EEG/PPG values within physiologically plausible ranges
- **Simulated device connection** - Bypasses actual Bluetooth device discovery
- **Full exercise flow** - All exercise phases work normally (baseline, active, post)
- **UI testing** - Complete QML interface can be tested
- **Database operations** - Sessions are saved to database normally
- **CSV export** - Metrics are exported with "DEMO" marker

**Implementation Details:**
- `SettingsController` manages `demoMode` setting via QSettings (persists between app launches)
- `MetricsModel` generates random values when `demoMode=true`:
  ```cpp
  if (settingsController->isDemoMode()) {
      metrics.concentration = 0.3 + (rand() % 40) / 100.0;  // 0.3-0.7
      metrics.alpha = 5.0 + (rand() % 10);  // 5-15 µV
      // ... other metrics
  }
  ```
- All features work except real hardware connection and actual EEG signal processing

**Use Cases:**
- UI/UX development without device
- Testing exercise logic and scoring algorithms
- Debugging database and CSV export
- Demonstrating application to stakeholders
- Continuous integration testing (future)

**Limitations:**
- Metrics are random, not based on actual brain states
- Cannot test device-specific issues (battery, Bluetooth, firmware)
- Calibration process is simulated (doesn't affect metrics)

### Manual Testing Workflow

With physical Neiry device:
1. Connect Neiry device (Band/Buds/Headphones)
2. Ensure battery > 20%
3. Run application
4. Verify device discovery and connection
5. Start exercise and monitor real-time metrics
6. Check CSV export in `data/` directory
7. Verify session saved in `data/bronnikov_sessions.db`

With Demo Mode (no device):
1. Enable Demo Mode in Settings
2. Select any exercise
3. Observe random but realistic metrics
4. Verify UI responsiveness and phase transitions
5. Check database and CSV exports contain "DEMO" marker

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

### Documentation

- **CapsuleAPI Doxygen**: `../CapsuleAPI/Docs/html/index.html` - Complete API reference
- **Bronnikov Methodology**: `/Users/avdemkin/code_vibe/Neuro/Docs_SBF/Metodichka_2-3_Stupeni.doc` - Official 2011 methodology document (source for all 78 exercises)
- **Product Specification**: `../PRD.md` - Full product requirements and UI/UX flows
- **Project Roadmap**: `ROADMAP.md` - Development history and future plans
- **Build Success Logs**: `BUILD_SUCCESS.md` and `BUILD_SUCCESS_GUI.md` - Platform-specific build instructions and troubleshooting

### Runtime Data Locations

**Database:**
- Path: `data/bronnikov_sessions.db`
- Schema: SQLite with users, sessions, and stage_progress tables
- Created automatically on first run

**CSV Exports:**
- Pattern: `data/session_YYYYMMDD_HHMMSS.csv`
- Contains: Timestamped metrics from all three phases (baseline, active, post)
- Format: CSV with headers (timestamp, concentration, relaxation, alpha, beta, theta, etc.)

**Application Logs:**
- Directory: `logs/`
- Pattern: `app_YYYYMMDD_HHMMSS.log`
- Contains: Device connection events, classifier initialization, errors, warnings

**Settings:**
- Location: Platform-dependent (QSettings)
  - macOS: `~/Library/Preferences/com.neiry.BronnikovExerciseApp.plist`
  - Windows: Registry `HKEY_CURRENT_USER\Software\Neiry\BronnikovExerciseApp`
- Contains: User preferences, demo mode flag, last selected profile

### Build Artifacts

**macOS:**
- Executable: `build/Release/BronnikovExerciseApp` (GUI) and `BronnikovExerciseAppCLI` (CLI)
- Libraries: `build/Release/libCapsuleClient.dylib` (copied from `../CapsuleAPI/Mac/`)
- Resources: `build/Release/BronnikovExerciseApp.app/Contents/Resources/` (QML files, assets)

**Windows:**
- Executable: `build\Release\BronnikovExerciseApp.exe` (GUI) and `BronnikovExerciseAppCLI.exe` (CLI)
- Libraries: `build\Release\CapsuleClient.dll` (copied from `../CapsuleAPI/Win/build/Release/`)
- Qt DLLs: Auto-deployed by windeployqt (Qt6Core.dll, Qt6Qml.dll, etc.)

### Testing Scripts

All scripts located in project root:

- **`./run_test.sh`** - Quick test launch with automatic timestamped logging
- **`./test_device.sh`** - Device connection verification (scans for Neiry devices)
- **`./test_settings.sh`** - Database schema and user profile verification
- **`./monitor_logs.sh`** - Real-time log monitoring with grep filtering

**Usage examples:**
```bash
# Quick test
./run_test.sh

# Check if device is discoverable
./test_device.sh

# Verify database is properly initialized
./test_settings.sh

# Monitor logs for errors
./monitor_logs.sh | grep ERROR
```
