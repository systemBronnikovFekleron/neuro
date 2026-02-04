cmake_minimum_required(VERSION 3.20)
project(BronnikovExerciseApp VERSION 0.10.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(BUILD_GUI "Build GUI version with Qt" ON)
option(BUILD_CLI "Build CLI version" ON)

# Platform detection
if(APPLE)
    set(PLATFORM_NAME "Mac")
    set(CAPSULE_LIB_PATH "${CMAKE_SOURCE_DIR}/../CapsuleAPI/Mac/libCapsuleClient.dylib")
elseif(WIN32)
    set(PLATFORM_NAME "Win")
    set(CAPSULE_LIB_PATH "${CMAKE_SOURCE_DIR}/../CapsuleAPI/Win/build/Release/CapsuleClient.dll")
endif()

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/../CapsuleAPI/${PLATFORM_NAME}/Include
    ${CMAKE_SOURCE_DIR}/src/gui  # For Qt models and controllers
)

# ============================================================================
# Qt Configuration (only if BUILD_GUI is ON)
# ============================================================================
if(BUILD_GUI)
    # Find Qt6 packages
    find_package(Qt6 6.7 REQUIRED COMPONENTS
        Core
        Gui
        Quick
        Qml
        Charts
        Svg
    )

    # Enable Qt MOC, UIC, RCC
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    # QML import path (for Qt Creator)
    set(QML_IMPORT_PATH "${CMAKE_SOURCE_DIR}/qml" CACHE STRING "Qt Creator QML import path")

    message(STATUS "Qt6 found: ${Qt6_DIR}")
    message(STATUS "Building GUI version")
endif()

# ============================================================================
# Source Files
# ============================================================================

# CapsuleAPI integration layer
set(CAPSULE_SOURCES
    src/capsule/CapsuleManager.cpp
    src/capsule/SessionManager.cpp
    src/capsule/MetricsCollector.cpp
)

# Exercise implementations
set(EXERCISE_SOURCES
    # Base class
    src/exercises/Exercise.cpp

    # Подготовительная ступень (10 упражнений)
    src/exercises/EnergyBall.cpp
    src/exercises/BallMovement.cpp
    src/exercises/FingerRays.cpp
    src/exercises/MindBreathing.cpp
    src/exercises/FastWind.cpp
    src/exercises/EyeRays.cpp
    src/exercises/SubtleBody.cpp
    src/exercises/HeavenEarth.cpp
    src/exercises/HeavyLight.cpp
    src/exercises/VolumePerception.cpp

    # 1-я ступень: Экология духа (18 упражнений - ПОЛНЫЙ КОМПЛЕКТ из методички 2011)
    src/exercises/MorphologicalFields.cpp
    src/exercises/PhantomSensations.cpp
    src/exercises/VerticalPumping.cpp
    src/exercises/HeavinessLightness.cpp
    src/exercises/EnergySeedIntroduction.cpp
    src/exercises/HeadPumping.cpp
    src/exercises/SpiritImpulse.cpp
    src/exercises/TibetanMassage.cpp
    src/exercises/SmallDragon.cpp
    src/exercises/BellSwing.cpp
    src/exercises/EnergeticCocoon.cpp
    src/exercises/EyePumping.cpp
    src/exercises/HorizontalPumping.cpp
    src/exercises/ThoughtBreathing.cpp
    src/exercises/ObjectWork.cpp
    src/exercises/AnatomicalAtlas.cpp
    src/exercises/AbstractModels.cpp
    src/exercises/ScreenActivation.cpp

    # 2-я ступень: Зрение вне глаз (25 упражнений - ПОЛНЫЙ КОМПЛЕКТ)
    src/exercises/PositivePsychology.cpp
    src/exercises/EnergyHandsStage1.cpp
    src/exercises/SensingPhantom.cpp
    src/exercises/ScreenControl.cpp
    src/exercises/ScreenColoring.cpp
    src/exercises/NumberTable.cpp
    src/exercises/ColoredCellsTable.cpp
    src/exercises/ColoredPicturesTable.cpp
    src/exercises/SpatialMemory.cpp
    src/exercises/RouteMemory.cpp
    src/exercises/HandColorVision.cpp
    src/exercises/HandSymbolVision.cpp
    src/exercises/HandTextVision.cpp
    src/exercises/BodyPartVision.cpp
    src/exercises/InternalOrgansVision.cpp
    src/exercises/ObjectVision.cpp
    src/exercises/ImageTransformation.cpp
    src/exercises/ColoredTextDrawing.cpp
    src/exercises/TextRotation.cpp
    src/exercises/DepthVision.cpp
    src/exercises/BlindMovement.cpp
    src/exercises/RemoteVision.cpp
    src/exercises/DarkVision.cpp
    src/exercises/WallVision.cpp
    src/exercises/SphereVision.cpp

    # 3-я ступень: Экран внутреннего видения (15 упражнений - ПОЛНЫЙ КОМПЛЕКТ)
    src/exercises/BiocomputerActivation.cpp
    src/exercises/InformationLoading.cpp
    src/exercises/VirtualKeyboard.cpp
    src/exercises/InformationSearch.cpp
    src/exercises/MemoryPalace.cpp
    src/exercises/ImageProcessing.cpp
    src/exercises/MathCalculations.cpp
    src/exercises/LanguageTranslation.cpp
    src/exercises/TimeManagement.cpp
    src/exercises/CreativeVisualization.cpp
    src/exercises/MusicCreation.cpp
    src/exercises/ProblemSolving.cpp
    src/exercises/FuturePrediction.cpp
    src/exercises/DataEncryption.cpp
    src/exercises/BiocomputerMastery.cpp

    # 4-я ступень: Радарное видение (10 упражнений - ПОЛНЫЙ КОМПЛЕКТ)
    src/exercises/EnergyFieldScanning.cpp
    src/exercises/AuraReading.cpp
    src/exercises/ChakraPerception.cpp
    src/exercises/OrganDiagnostics.cpp
    src/exercises/ObjectHistory.cpp
    src/exercises/DistantHealing.cpp
    src/exercises/InformationFieldAccess.cpp
    src/exercises/PastLifeVision.cpp
    src/exercises/MultidimensionalPerception.cpp
    src/exercises/CosmicConsciousness.cpp

    # Exercise library
    src/exercises/ExerciseLibrary.cpp
)

# Session management
set(SESSION_SOURCES
    src/session/ExerciseSession.cpp
    src/session/GuidedInstructor.cpp
    src/session/PhaseController.cpp
)

# Real-time feedback
set(FEEDBACK_SOURCES
    src/feedback/RealTimeFeedback.cpp
    src/feedback/MetricsAnalyzer.cpp
    src/feedback/ProgressTracker.cpp
)

# Database
set(DATABASE_SOURCES
    src/database/SessionDatabase.cpp
    src/database/UserProfile.cpp
    src/database/SessionStorage.cpp
)

# Analytics
set(ANALYTICS_SOURCES
    src/analytics/TrendAnalyzer.cpp
    src/analytics/ReportGenerator.cpp
)

# GUI-specific sources (Qt models and controllers)
if(BUILD_GUI)
    set(GUI_SOURCES
        src/gui/models/ExerciseModel.cpp
        src/gui/models/MetricsModel.cpp
        src/gui/models/SessionModel.cpp
        src/gui/controllers/DeviceController.cpp
        src/gui/controllers/ExerciseController.cpp
        src/gui/main_gui.cpp
    )

    # QML resources
    set(QML_RESOURCES
        qml/qml.qrc
    )
endif()

# CLI-specific sources
if(BUILD_CLI)
    set(CLI_SOURCES
        src/cli/main_cli.cpp
    )
endif()

# Common sources (used by both CLI and GUI)
set(COMMON_SOURCES
    ${CAPSULE_SOURCES}
    ${EXERCISE_SOURCES}
    ${SESSION_SOURCES}
    ${FEEDBACK_SOURCES}
    ${DATABASE_SOURCES}
    ${ANALYTICS_SOURCES}
)

# ============================================================================
# Executables
# ============================================================================

# GUI executable
if(BUILD_GUI)
    add_executable(BronnikovExerciseApp
        ${COMMON_SOURCES}
        ${GUI_SOURCES}
        ${QML_RESOURCES}
    )

    # Link Qt libraries
    target_link_libraries(BronnikovExerciseApp PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Quick
        Qt6::Qml
        Qt6::Charts
        Qt6::Svg
        ${CAPSULE_LIB_PATH}
        SQLite::SQLite3
    )

    # Set GUI application properties
    if(APPLE)
        set_target_properties(BronnikovExerciseApp PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.bronnikov.exerciseapp"
            MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
            XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
            XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO"
            XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO"
        )
    elseif(WIN32)
        set_target_properties(BronnikovExerciseApp PROPERTIES
            WIN32_EXECUTABLE TRUE
        )
    endif()
endif()

# CLI executable
if(BUILD_CLI)
    add_executable(BronnikovExerciseAppCLI
        ${COMMON_SOURCES}
        ${CLI_SOURCES}
    )

    target_link_libraries(BronnikovExerciseAppCLI PRIVATE
        ${CAPSULE_LIB_PATH}
        SQLite::SQLite3
    )
endif()

# ============================================================================
# Dependencies
# ============================================================================

# Find SQLite3
find_package(SQLite3 REQUIRED)

# ============================================================================
# Post-build steps
# ============================================================================

# Copy CapsuleAPI library to output directory
if(APPLE)
    if(BUILD_GUI)
        add_custom_command(TARGET BronnikovExerciseApp POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CAPSULE_LIB_PATH}
            $<TARGET_FILE_DIR:BronnikovExerciseApp>
        )
    endif()

    if(BUILD_CLI)
        add_custom_command(TARGET BronnikovExerciseAppCLI POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CAPSULE_LIB_PATH}
            $<TARGET_FILE_DIR:BronnikovExerciseAppCLI>
        )
    endif()
endif()

# ============================================================================
# Compiler settings
# ============================================================================

# Compiler warnings
if(MSVC)
    if(BUILD_GUI)
        target_compile_options(BronnikovExerciseApp PRIVATE /W4)
    endif()
    if(BUILD_CLI)
        target_compile_options(BronnikovExerciseAppCLI PRIVATE /W4)
    endif()
else()
    if(BUILD_GUI)
        target_compile_options(BronnikovExerciseApp PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    if(BUILD_CLI)
        target_compile_options(BronnikovExerciseAppCLI PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()

# ============================================================================
# Summary
# ============================================================================

message(STATUS "==============================================")
message(STATUS "Bronnikov Exercise App Configuration Summary")
message(STATUS "==============================================")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Platform: ${PLATFORM_NAME}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build GUI: ${BUILD_GUI}")
message(STATUS "Build CLI: ${BUILD_CLI}")
if(BUILD_GUI)
    message(STATUS "Qt6 Version: ${Qt6_VERSION}")
endif()
message(STATUS "CapsuleAPI Library: ${CAPSULE_LIB_PATH}")
message(STATUS "==============================================")
