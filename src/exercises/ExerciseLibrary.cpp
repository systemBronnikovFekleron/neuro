#include "ExerciseLibrary.h"

// Подготовительная ступень (10 упражнений)
#include "EnergyBall.h"
#include "BallMovement.h"
#include "FingerRays.h"
#include "MindBreathing.h"
#include "FastWind.h"
#include "EyeRays.h"
#include "SubtleBody.h"
#include "HeavenEarth.h"
#include "HeavyLight.h"
#include "VolumePerception.h"

// 1-я ступень "Экология духа" (18 упражнений)
#include "MorphologicalFields.h"
#include "PhantomSensations.h"
#include "VerticalPumping.h"
#include "HeavinessLightness.h"
#include "EnergySeedIntroduction.h"
#include "HeadPumping.h"
#include "SpiritImpulse.h"
#include "TibetanMassage.h"
#include "SmallDragon.h"
#include "BellSwing.h"
#include "EnergeticCocoon.h"
#include "EyePumping.h"
#include "HorizontalPumping.h"
#include "ThoughtBreathing.h"
#include "ObjectWork.h"
#include "AnatomicalAtlas.h"
#include "AbstractModels.h"
#include "ScreenActivation.h"

// 2-я ступень "Зрение вне глаз" (25 упражнений)
#include "PositivePsychology.h"
#include "EnergyHandsStage1.h"
#include "SensingPhantom.h"
#include "ScreenControl.h"
#include "ScreenColoring.h"
#include "NumberTable.h"
#include "ColoredCellsTable.h"
#include "ColoredPicturesTable.h"
#include "SpatialMemory.h"
#include "RouteMemory.h"
#include "HandColorVision.h"
#include "HandSymbolVision.h"
#include "HandTextVision.h"
#include "BodyPartVision.h"
#include "InternalOrgansVision.h"
#include "ObjectVision.h"
#include "ImageTransformation.h"
#include "ColoredTextDrawing.h"
#include "TextRotation.h"
#include "DepthVision.h"
#include "BlindMovement.h"
#include "RemoteVision.h"
#include "DarkVision.h"
#include "WallVision.h"
#include "SphereVision.h"

// 3-я ступень "Экран внутреннего видения" (15 упражнений)
#include "BiocomputerActivation.h"
#include "InformationLoading.h"
#include "VirtualKeyboard.h"
#include "ImageProcessing.h"
#include "CreativeVisualization.h"
#include "MemoryPalace.h"
#include "MathCalculations.h"
#include "InformationSearch.h"
#include "LanguageTranslation.h"
#include "ProblemSolving.h"
#include "TimeManagement.h"
#include "MusicCreation.h"
#include "DataEncryption.h"
#include "FuturePrediction.h"
#include "BiocomputerMastery.h"

// 4-я ступень "Радарное видение" (10 упражнений)
#include "EnergyFieldScanning.h"
#include "AuraReading.h"
#include "ChakraPerception.h"
#include "OrganDiagnostics.h"
#include "DistantHealing.h"
#include "InformationFieldAccess.h"
#include "PastLifeVision.h"
#include "ObjectHistory.h"
#include "MultidimensionalPerception.h"
#include "CosmicConsciousness.h"

#include <iostream>
#include <limits>
#include <algorithm>

namespace Bronnikov {

ExerciseLibrary::ExerciseLibrary() {
    registerExercises();
    buildStageIndex();  // НОВОЕ: строим индекс после регистрации
}

void ExerciseLibrary::registerExercises() {
    // Подготовительная ступень (10 упражнений)
    m_exercises.push_back(std::make_shared<EnergyBall>());
    m_exercises.push_back(std::make_shared<BallMovement>());
    m_exercises.push_back(std::make_shared<FingerRays>());
    m_exercises.push_back(std::make_shared<MindBreathing>());
    m_exercises.push_back(std::make_shared<FastWind>());
    m_exercises.push_back(std::make_shared<EyeRays>());
    m_exercises.push_back(std::make_shared<SubtleBody>());
    m_exercises.push_back(std::make_shared<HeavenEarth>());
    m_exercises.push_back(std::make_shared<HeavyLight>());
    m_exercises.push_back(std::make_shared<VolumePerception>());

    // 1-я ступень "Экология духа" (18 упражнений)
    m_exercises.push_back(std::make_shared<MorphologicalFields>());
    m_exercises.push_back(std::make_shared<PhantomSensations>());
    m_exercises.push_back(std::make_shared<VerticalPumping>());
    m_exercises.push_back(std::make_shared<HeavinessLightness>());
    m_exercises.push_back(std::make_shared<EnergySeedIntroduction>());
    m_exercises.push_back(std::make_shared<HeadPumping>());
    m_exercises.push_back(std::make_shared<SpiritImpulse>());
    m_exercises.push_back(std::make_shared<TibetanMassage>());
    m_exercises.push_back(std::make_shared<SmallDragon>());
    m_exercises.push_back(std::make_shared<BellSwing>());
    m_exercises.push_back(std::make_shared<EnergeticCocoon>());
    m_exercises.push_back(std::make_shared<EyePumping>());
    m_exercises.push_back(std::make_shared<HorizontalPumping>());
    m_exercises.push_back(std::make_shared<ThoughtBreathing>());
    m_exercises.push_back(std::make_shared<ObjectWork>());
    m_exercises.push_back(std::make_shared<AnatomicalAtlas>());
    m_exercises.push_back(std::make_shared<AbstractModels>());
    m_exercises.push_back(std::make_shared<ScreenActivation>());

    // 2-я ступень "Зрение вне глаз" (25 упражнений)
    m_exercises.push_back(std::make_shared<PositivePsychology>());
    m_exercises.push_back(std::make_shared<EnergyHandsStage1>());
    m_exercises.push_back(std::make_shared<SensingPhantom>());
    m_exercises.push_back(std::make_shared<ScreenControl>());
    m_exercises.push_back(std::make_shared<ScreenColoring>());
    m_exercises.push_back(std::make_shared<NumberTable>());
    m_exercises.push_back(std::make_shared<ColoredCellsTable>());
    m_exercises.push_back(std::make_shared<ColoredPicturesTable>());
    m_exercises.push_back(std::make_shared<SpatialMemory>());
    m_exercises.push_back(std::make_shared<RouteMemory>());
    m_exercises.push_back(std::make_shared<HandColorVision>());
    m_exercises.push_back(std::make_shared<HandSymbolVision>());
    m_exercises.push_back(std::make_shared<HandTextVision>());
    m_exercises.push_back(std::make_shared<BodyPartVision>());
    m_exercises.push_back(std::make_shared<InternalOrgansVision>());
    m_exercises.push_back(std::make_shared<ObjectVision>());
    m_exercises.push_back(std::make_shared<ImageTransformation>());
    m_exercises.push_back(std::make_shared<ColoredTextDrawing>());
    m_exercises.push_back(std::make_shared<TextRotation>());
    m_exercises.push_back(std::make_shared<DepthVision>());
    m_exercises.push_back(std::make_shared<BlindMovement>());
    m_exercises.push_back(std::make_shared<RemoteVision>());
    m_exercises.push_back(std::make_shared<DarkVision>());
    m_exercises.push_back(std::make_shared<WallVision>());
    m_exercises.push_back(std::make_shared<SphereVision>());

    // 3-я ступень "Экран внутреннего видения" (15 упражнений)
    m_exercises.push_back(std::make_shared<BiocomputerActivation>());
    m_exercises.push_back(std::make_shared<InformationLoading>());
    m_exercises.push_back(std::make_shared<VirtualKeyboard>());
    m_exercises.push_back(std::make_shared<ImageProcessing>());
    m_exercises.push_back(std::make_shared<CreativeVisualization>());
    m_exercises.push_back(std::make_shared<MemoryPalace>());
    m_exercises.push_back(std::make_shared<MathCalculations>());
    m_exercises.push_back(std::make_shared<InformationSearch>());
    m_exercises.push_back(std::make_shared<LanguageTranslation>());
    m_exercises.push_back(std::make_shared<ProblemSolving>());
    m_exercises.push_back(std::make_shared<TimeManagement>());
    m_exercises.push_back(std::make_shared<MusicCreation>());
    m_exercises.push_back(std::make_shared<DataEncryption>());
    m_exercises.push_back(std::make_shared<FuturePrediction>());
    m_exercises.push_back(std::make_shared<BiocomputerMastery>());

    // 4-я ступень "Радарное видение" (10 упражнений)
    m_exercises.push_back(std::make_shared<EnergyFieldScanning>());
    m_exercises.push_back(std::make_shared<AuraReading>());
    m_exercises.push_back(std::make_shared<ChakraPerception>());
    m_exercises.push_back(std::make_shared<OrganDiagnostics>());
    m_exercises.push_back(std::make_shared<DistantHealing>());
    m_exercises.push_back(std::make_shared<InformationFieldAccess>());
    m_exercises.push_back(std::make_shared<PastLifeVision>());
    m_exercises.push_back(std::make_shared<ObjectHistory>());
    m_exercises.push_back(std::make_shared<MultidimensionalPerception>());
    m_exercises.push_back(std::make_shared<CosmicConsciousness>());
}

const std::vector<std::shared_ptr<Exercise>>& ExerciseLibrary::getExercises() const {
    return m_exercises;
}

std::shared_ptr<Exercise> ExerciseLibrary::getExercise(size_t index) const {
    if (index >= m_exercises.size()) {
        return nullptr;
    }
    return m_exercises[index];
}

std::shared_ptr<Exercise> ExerciseLibrary::showMenu() const {
    std::cout << "\n╔═══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║           Методика Бронникова - Выбор упражнения             ║" << std::endl;
    std::cout << "╚═══════════════════════════════════════════════════════════════╝" << std::endl;

    auto available_stages = getAvailableStages();

    int global_index = 1;  // Сквозная нумерация для ввода
    std::map<int, std::shared_ptr<Exercise>> index_to_exercise;

    for (ExerciseStage stage : available_stages) {
        auto exercises = getExercisesByStage(stage);

        if (exercises.empty()) continue;

        // Заголовок ступени
        std::cout << "\n┌─ " << getStageDisplayName(stage)
                  << " (" << exercises.size() << " упр.) ────" << std::endl;

        // Упражнения
        for (const auto& ex : exercises) {
            std::cout << "  [" << global_index << "] "
                      << ex->getStageId() << " " << ex->getName()
                      << " (" << ex->getDuration() << " мин)" << std::endl;

            if (global_index == 1) {
                std::cout << "      ⭐ Рекомендовано для начинающих" << std::endl;
            }

            index_to_exercise[global_index] = ex;
            global_index++;
        }

        std::cout << "└────────────────────────────────────────────────────────────────" << std::endl;
    }

    std::cout << "\n  [0] Выход из программы" << std::endl;
    std::cout << "\n═══════════════════════════════════════════════════════════════" << std::endl;
    std::cout << "Ваш выбор (0-" << (global_index - 1) << "): ";

    int choice;
    while (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "⚠️  Некорректный ввод. Введите число от 0 до "
                  << (global_index - 1) << ": ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0) {
        return nullptr;
    }

    auto it = index_to_exercise.find(choice);
    if (it != index_to_exercise.end()) {
        return it->second;
    }

    std::cout << "⚠️  Неверный выбор! Попробуйте снова." << std::endl;
    return showMenu();
}

std::shared_ptr<Exercise> ExerciseLibrary::getRecommended() const {
    // Для новичков рекомендуем первое упражнение - "Энергетический шар"
    return m_exercises.empty() ? nullptr : m_exercises[0];
}

// НОВЫЕ МЕТОДЫ для поддержки ступеней

void ExerciseLibrary::buildStageIndex() {
    m_exercises_by_stage.clear();

    for (const auto& exercise : m_exercises) {
        ExerciseStage stage = exercise->getStage();
        m_exercises_by_stage[stage].push_back(exercise);
    }

    // Сортировка по order_in_stage внутри каждой ступени
    for (auto& [stage, exercises] : m_exercises_by_stage) {
        std::sort(exercises.begin(), exercises.end(),
            [](const auto& a, const auto& b) {
                return a->getOrderInStage() < b->getOrderInStage();
            });
    }
}

std::vector<std::shared_ptr<Exercise>> ExerciseLibrary::getExercisesByStage(ExerciseStage stage) const {
    auto it = m_exercises_by_stage.find(stage);
    if (it != m_exercises_by_stage.end()) {
        return it->second;
    }
    return {};
}

std::vector<ExerciseStage> ExerciseLibrary::getAvailableStages() const {
    std::vector<ExerciseStage> stages;
    for (const auto& [stage, exercises] : m_exercises_by_stage) {
        if (!exercises.empty()) {
            stages.push_back(stage);
        }
    }
    // Сортировка по порядку ступеней
    std::sort(stages.begin(), stages.end());
    return stages;
}

size_t ExerciseLibrary::getStageExerciseCount(ExerciseStage stage) const {
    auto it = m_exercises_by_stage.find(stage);
    return (it != m_exercises_by_stage.end()) ? it->second.size() : 0;
}

std::shared_ptr<Exercise> ExerciseLibrary::getExerciseByStageOrder(
    ExerciseStage stage, int order) const {

    auto exercises = getExercisesByStage(stage);
    for (const auto& ex : exercises) {
        if (ex->getOrderInStage() == order) {
            return ex;
        }
    }
    return nullptr;
}

std::shared_ptr<Exercise> ExerciseLibrary::getExerciseByName(const std::string& name) const {
    for (const auto& ex : m_exercises) {
        if (ex->getName() == name) {
            return ex;
        }
    }
    return nullptr;
}

} // namespace Bronnikov
