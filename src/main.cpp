#include "CapsuleManager.h"
#include "SessionManager.h"
#include "MetricsCollector.h"
#include "ExerciseLibrary.h"
#include "SessionDatabase.h"

#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <csignal>
#include <atomic>
#include <numeric>
#include <algorithm>

using namespace Bronnikov;

// Флаг для выхода из программы
std::atomic<bool> g_should_exit(false);

// Счетчик нажатий Ctrl+C (для принудительного выхода)
std::atomic<int> g_interrupt_count(0);

void signalHandler(int signal) {
    g_interrupt_count++;

    if (g_interrupt_count == 1) {
        // Первое нажатие - graceful shutdown
        std::cout << "\n\n⚠️  Получен сигнал завершения. Останавливаем сессию..." << std::endl;
        std::cout << "    (Нажмите Ctrl+C еще раз для немедленного выхода)" << std::endl;
        g_should_exit = true;
    } else {
        // Второе нажатие - принудительный выход
        std::cout << "\n\n🛑 Принудительное завершение!" << std::endl;
        std::exit(1);
    }
}

void printMetrics(const MetricsSnapshot& metrics) {
    std::cout << "\n=== Текущие метрики ===" << std::endl;
    std::cout << "Альфа:         " << metrics.alpha_power << std::endl;
    std::cout << "Бета:          " << metrics.beta_power << std::endl;
    std::cout << "Тета:          " << metrics.theta_power << std::endl;
    std::cout << "Концентрация:  " << metrics.concentration << std::endl;
    std::cout << "Релаксация:    " << metrics.relaxation << std::endl;
    std::cout << "Усталость:     " << metrics.fatigue << std::endl;
    std::cout << "Фокус:         " << metrics.focus << std::endl;
    std::cout << "Стресс:        " << metrics.stress << std::endl;
    std::cout << "Пульс:         " << metrics.heart_rate << " уд/мин" << std::endl;
    std::cout << "========================" << std::endl;
}

/**
 * @brief Показать статистику по конкретному упражнению
 */
void showStatistics(SessionDatabase& db, const std::string& exercise_name) {
    std::cout << "\n╔═══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  Статистика по упражнению: " << exercise_name;
    // Добавляем пробелы для выравнивания
    int padding = 64 - 30 - exercise_name.length();
    for (int i = 0; i < padding; ++i) std::cout << " ";
    std::cout << "║" << std::endl;
    std::cout << "╚═══════════════════════════════════════════════════════════════╝" << std::endl;

    auto sessions = db.getSessionsByExercise(exercise_name);

    if (sessions.empty()) {
        std::cout << "\n📝 Это была ваша первая сессия!" << std::endl;
        std::cout << "   Продолжайте практиковать для отслеживания прогресса." << std::endl;
        return;
    }

    std::cout << "\n✓ Всего выполнено сессий: " << sessions.size() << std::endl;

    // Последние 5 сессий
    std::cout << "\n📊 Последние сессии:" << std::endl;
    std::cout << "   ┌─────────────────────┬──────────┬────────────┬──────────┐" << std::endl;
    std::cout << "   │ Дата                │ Успех    │ Длительн.  │ Концентр.│" << std::endl;
    std::cout << "   ├─────────────────────┼──────────┼────────────┼──────────┤" << std::endl;

    for (size_t i = 0; i < std::min(size_t(5), sessions.size()); ++i) {
        const auto& s = sessions[i];
        char buffer[128];
        snprintf(buffer, sizeof(buffer), "   │ %-19s │ %6.1f%% │ %7d сек│ %6.1f   │",
                 s.timestamp.c_str(),
                 s.success_rate,
                 s.duration_seconds,
                 s.avg_concentration);
        std::cout << buffer << std::endl;
    }
    std::cout << "   └─────────────────────┴──────────┴────────────┴──────────┘" << std::endl;

    // Тренд Success Rate
    if (sessions.size() >= 2) {
        double first_sr = sessions.back().success_rate;  // Самая старая
        double last_sr = sessions.front().success_rate;   // Самая новая
        double change = last_sr - first_sr;

        std::cout << "\n📈 Динамика успеха (первая → последняя сессия): ";
        if (change > 0) {
            std::cout << std::showpos << change << std::noshowpos << "% ✓ (улучшение!)";
        } else if (change < 0) {
            std::cout << std::showpos << change << std::noshowpos << "% ⚠️  (снижение)";
        } else {
            std::cout << "без изменений";
        }
        std::cout << std::endl;
    }

    // Лучший результат
    auto best = std::max_element(sessions.begin(), sessions.end(),
        [](const auto& a, const auto& b) { return a.success_rate < b.success_rate; });

    std::cout << "\n🏆 Лучший результат: " << best->success_rate << "% "
              << "(" << best->timestamp << ")" << std::endl;

    // Средние значения
    double avg_success = 0.0;
    double avg_concentration = 0.0;
    double avg_relaxation = 0.0;
    int count = 0;

    for (const auto& s : sessions) {
        avg_success += s.success_rate;
        avg_concentration += s.avg_concentration;
        avg_relaxation += s.avg_relaxation;
        ++count;
    }

    if (count > 0) {
        avg_success /= count;
        avg_concentration /= count;
        avg_relaxation /= count;

        std::cout << "\n📊 Средние показатели:" << std::endl;
        std::cout << "   Успех:        " << avg_success << "%" << std::endl;
        std::cout << "   Концентрация: " << avg_concentration << std::endl;
        std::cout << "   Релаксация:   " << avg_relaxation << std::endl;
    }
}

/**
 * @brief Показать общую статистику пользователя
 */
void showUserStats(SessionDatabase& db) {
    std::cout << "\n╔═══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║              Ваша общая статистика практики                   ║" << std::endl;
    std::cout << "╚═══════════════════════════════════════════════════════════════╝" << std::endl;

    auto stats = db.getUserStats();

    if (stats.total_sessions == 0) {
        std::cout << "\n📝 У вас пока нет завершенных сессий." << std::endl;
        std::cout << "   Начните практиковать упражнения для накопления статистики!" << std::endl;
        return;
    }

    std::cout << "\n✓ Всего сессий:         " << stats.total_sessions << std::endl;
    std::cout << "⏱  Всего часов практики: " << std::fixed << std::setprecision(1)
              << stats.total_hours << " ч" << std::endl;
    std::cout << "📊 Средний успех:        " << std::fixed << std::setprecision(1)
              << stats.avg_success_rate << "%" << std::endl;
    std::cout << "📅 Первая сессия:        " << stats.first_session_date << std::endl;
    std::cout << "📅 Последняя сессия:     " << stats.last_session_date << std::endl;

    // Статистика по упражнениям
    std::cout << "\n📊 Статистика по упражнениям:" << std::endl;
    std::cout << "   ┌──────────────────────────────────┬───────┬──────────┐" << std::endl;
    std::cout << "   │ Упражнение                       │ Сессий│ Ср. успех│" << std::endl;
    std::cout << "   ├──────────────────────────────────┼───────┼──────────┤" << std::endl;

    // Список всех упражнений
    std::vector<std::string> exercise_names = {
        "Энергетический шар",
        "Движение шара",
        "Лучики пальцев",
        "Дыхание мыслью",
        "Быстрый ветер",
        "Лучики глаз",
        "Тонкое тело",
        "Небо и Земля",
        "Тяжелое и Легкое",
        "Объемное восприятие"
    };

    for (const auto& name : exercise_names) {
        auto exercise_sessions = db.getSessionsByExercise(name);
        if (!exercise_sessions.empty()) {
            double avg_sr = 0.0;
            for (const auto& s : exercise_sessions) {
                avg_sr += s.success_rate;
            }
            avg_sr /= exercise_sessions.size();

            char buffer[128];
            snprintf(buffer, sizeof(buffer), "   │ %-32s │ %5zu │ %6.1f%% │",
                     name.c_str(),
                     exercise_sessions.size(),
                     avg_sr);
            std::cout << buffer << std::endl;
        }
    }

    std::cout << "   └──────────────────────────────────┴───────┴──────────┘" << std::endl;

    // Прогресс за последние дни (опционально)
    // TODO: Добавить расчет streak (дни подряд)
}

// НОВАЯ ФУНКЦИЯ: Показать прогресс по ступеням
void showStageProgress(SessionDatabase& db) {
    std::cout << "\n╔═══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║              Прогресс по ступеням обучения                    ║" << std::endl;
    std::cout << "╚═══════════════════════════════════════════════════════════════╝" << std::endl;

    auto all_progress = db.getAllStagesProgress();
    bool has_any_progress = false;

    for (const auto& progress : all_progress) {
        if (progress.total_sessions == 0) {
            continue;  // Пропускаем ступени без сессий
        }

        has_any_progress = true;

        std::cout << "\n┌─ " << getStageDisplayName(static_cast<ExerciseStage>(progress.stage))
                  << " ────" << std::endl;

        std::cout << "  Освоено упражнений: " << progress.completed_exercises << "/"
                  << progress.total_exercises_in_stage
                  << " (" << std::fixed << std::setprecision(1)
                  << progress.completion_percentage << "%)" << std::endl;

        std::cout << "  Всего сессий: " << progress.total_sessions << std::endl;

        std::cout << "  Средний успех: " << std::fixed << std::setprecision(1)
                  << progress.avg_success_rate << "%" << std::endl;

        // Прогресс-бар
        int bar_length = 40;
        int filled = static_cast<int>(progress.completion_percentage * bar_length / 100.0);
        std::cout << "  [";
        for (int i = 0; i < bar_length; ++i) {
            std::cout << ((i < filled) ? "█" : "░");
        }
        std::cout << "]" << std::endl;

        std::cout << "└────────────────────────────────────────────────────────────────" << std::endl;
    }

    if (!has_any_progress) {
        std::cout << "\n📝 У вас пока нет завершенных сессий." << std::endl;
        std::cout << "   Начните практиковать упражнения для отслеживания прогресса!" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // ВАЖНО: Блокируем SIGINT чтобы избежать abort() от CapsuleAPI
    // CapsuleAPI имеет встроенный stacktraceSignalHandler, который вызывает abort()
    // Блокируем сигнал полностью - пользователь должен дождаться завершения упражнения
    signal(SIGINT, SIG_IGN);   // Игнорируем Ctrl+C
    signal(SIGTERM, SIG_IGN);  // Игнорируем SIGTERM

    std::cout << "==================================================" << std::endl;
    std::cout << "  Приложение для практики упражнений Бронникова  " << std::endl;
    std::cout << "         с мониторингом ЭЭГ (Neiry Band)         " << std::endl;
    std::cout << "==================================================" << std::endl;

    // Создаем базу данных для хранения сессий и профилей (в начале main!)
    SessionDatabase db("data/bronnikov_sessions.db");
    if (!db.open()) {
        std::cerr << "⚠️  Не удалось открыть базу данных. Сессии не будут сохраняться." << std::endl;
    }

    // Загружаем профиль пользователя
    UserProfileData user_profile = db.loadUserProfile("default");
    if (user_profile.created_at.empty()) {
        std::cout << "📝 Создаем новый профиль пользователя..." << std::endl;
        user_profile.user_id = "default";
        user_profile.name = "Пользователь";
        db.saveUserProfile(user_profile);
    } else {
        std::cout << "✓ Профиль загружен: " << user_profile.name << std::endl;
        if (user_profile.iaf > 0.0) {
            std::cout << "  IAF: " << user_profile.iaf << " Hz" << std::endl;
        }
        if (user_profile.baseline_alpha > 0.0) {
            std::cout << "  Baseline сохранен (Alpha: " << user_profile.baseline_alpha << ")" << std::endl;
        }
    }

    // Создаем менеджер Capsule
    CapsuleManager capsule_manager;

    // Устанавливаем callbacks
    capsule_manager.setOnConnectedCallback([](bool success) {
        if (success) {
            std::cout << "✓ Успешное подключение к Capsule backend" << std::endl;
        } else {
            std::cout << "✗ Ошибка подключения к Capsule backend" << std::endl;
        }
    });

    capsule_manager.setOnDeviceConnectedCallback([](const std::string& device_name, bool success) {
        if (success) {
            std::cout << "✓ Подключено к устройству: " << device_name << std::endl;
        } else {
            std::cout << "✗ Ошибка подключения к устройству: " << device_name << std::endl;
        }
    });

    capsule_manager.setOnErrorCallback([](const std::string& error) {
        std::cout << "✗ Ошибка: " << error << std::endl;
    });

    capsule_manager.setOnBatteryUpdateCallback([](int level) {
        std::cout << "🔋 Батарея обновлена: " << level << "%" << std::endl;
    });

    // Подключаемся к Capsule backend
    std::cout << "\n[1/4] Подключение к Capsule backend..." << std::endl;
    if (!capsule_manager.connect("inproc://capsule")) {
        std::cerr << "Не удалось подключиться к Capsule. Выход." << std::endl;
        return 1;
    }

    // Обновляем клиент для обработки callback подключения
    for (int i = 0; i < 10 && !capsule_manager.isConnected(); ++i) {
        capsule_manager.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (!capsule_manager.isConnected()) {
        std::cerr << "Timeout при подключении к Capsule." << std::endl;
        return 1;
    }

    // Поиск устройств
    std::cout << "\n[2/4] Поиск устройств Neiry..." << std::endl;
    auto devices = capsule_manager.discoverDevices();

    if (devices.empty()) {
        std::cerr << "Устройства Neiry не найдены." << std::endl;
        std::cerr << "Убедитесь что устройство включено и находится рядом." << std::endl;
        return 1;
    }

    std::cout << "Найдено устройств: " << devices.size() << std::endl;
    for (size_t i = 0; i < devices.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << devices[i] << std::endl;
    }

    // Подключаемся к первому устройству
    std::cout << "\n[3/4] Подключение к устройству: " << devices[0] << "..." << std::endl;
    if (!capsule_manager.connectToDevice(devices[0])) {
        std::cerr << "Не удалось подключиться к устройству." << std::endl;
        return 1;
    }

    // Ожидаем подключения
    for (int i = 0; i < 50 && !capsule_manager.isDeviceConnected(); ++i) {
        capsule_manager.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (!capsule_manager.isDeviceConnected()) {
        std::cerr << "Timeout при подключении к устройству." << std::endl;
        return 1;
    }

    // Создаем менеджер сессий
    SessionManager session_manager(capsule_manager.getClient(), capsule_manager.getDevice());

    // Указатель на коллектор метрик (будет создан в callback)
    std::shared_ptr<MetricsCollector> metrics_collector_ptr;
    bool classifiers_ready = false;

    // КРИТИЧНО: Создаем классификаторы В CALLBACK onSessionStarted (как в CapsuleClientExample!)
    session_manager.setOnSessionStartedCallback([&](bool success) {
        if (!success) {
            std::cout << "✗ Ошибка запуска сессии" << std::endl;
            return;
        }

        std::cout << "✓ Сессия начата" << std::endl;
        session_manager.markActivity(static_cast<clCUserActivity>(0));

        metrics_collector_ptr = std::make_shared<MetricsCollector>(session_manager.getSession());

        if (!metrics_collector_ptr->initialize()) {
            std::cerr << "✗ Не удалось инициализировать классификаторы" << std::endl;
            return;
        }

        // Включаем логирование с автоматическим именем файла (timestamp)
        metrics_collector_ptr->enableLogging(true, "");

        // КРИТИЧНО: Переключаем режим устройства ПОСЛЕ создания классификаторов!
        // Включаем режим EEG Signal для мозговых волн
        clCDevice_SwitchMode(capsule_manager.getDevice(), clC_DM_Signal);

        // Включаем режим PPG для кардио метрик (пульс, стресс индекс)
        clCDevice_SwitchMode(capsule_manager.getDevice(), clC_DM_StartPPG);

        classifiers_ready = true;
    });

    // Запускаем сессию
    std::cout << "\n[4/4] Запуск EEG-сессии..." << std::endl;
    if (!session_manager.startSession("Упражнение: Энергетический шар")) {
        std::cerr << "Не удалось запустить сессию." << std::endl;
        return 1;
    }

    // Ожидаем активации сессии
    for (int i = 0; i < 50 && !session_manager.isActive(); ++i) {
        capsule_manager.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (!session_manager.isActive()) {
        std::cerr << "Timeout при запуске сессии." << std::endl;
        return 1;
    }

    // Ожидаем инициализацию классификаторов
    std::cout << "\nИнициализация классификаторов..." << std::endl;
    for (int i = 0; i < 100 && !classifiers_ready; ++i) {
        capsule_manager.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (!classifiers_ready || !metrics_collector_ptr) {
        std::cerr << "✗ Timeout: классификаторы не инициализированы" << std::endl;
        return 1;
    }

    // Дополнительное ожидание для стабилизации (КРИТИЧНО!)
    // Классификаторам нужно время на обработку первых данных
    std::cout << "\n⏳ Ожидание стабилизации классификаторов (60 секунд)..." << std::endl;
    std::cout << "   Это нормально - классификаторы собирают первичные данные." << std::endl;

    for (int i = 0; i < 600; ++i) {  // 60 секунд (600 * 100ms)
        capsule_manager.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Показываем прогресс каждую секунду
        if (i % 10 == 0 && i > 0) {
            std::cout << "   " << (i / 10) << " сек..." << std::flush;
            if (i % 50 == 0) std::cout << std::endl;
        }
    }

    std::cout << "\n✓ Все системы готовы!" << std::endl;
    std::cout << "\nБатарея устройства: " << capsule_manager.getBatteryLevel() << "%" << std::endl;
    std::cout << "Качество сигнала: " << capsule_manager.getSignalQuality() << "%" << std::endl;

    // Опциональная калибровка
    std::cout << "\nЗапустить калибровку NFB? (y/n): ";
    std::string response;
    std::getline(std::cin, response);

    if (response == "y" || response == "Y") {
        std::cout << "\nЗапуск калибровки (90 секунд)..." << std::endl;
        std::cout << "Сядьте удобно, закройте глаза и расслабьтесь." << std::endl;

        metrics_collector_ptr->setOnCalibrationCompleteCallback([&user_profile, &db](bool success, double iaf) {
            if (success) {
                std::cout << "\n✓ Калибровка завершена!" << std::endl;
                std::cout << "Ваша индивидуальная альфа-частота (IAF): " << iaf << " Hz" << std::endl;

                // Сохраняем результаты калибровки в профиль
                user_profile.iaf = iaf;
                // Обновляем дату калибровки
                auto now = std::chrono::system_clock::now();
                auto time_t_now = std::chrono::system_clock::to_time_t(now);
                std::tm tm_now;
                localtime_r(&time_t_now, &tm_now);
                char buffer[64];
                std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm_now);
                user_profile.last_calibration_date = buffer;

                if (db.isOpen()) {
                    db.saveUserProfile(user_profile);
                }
            } else {
                std::cout << "\n✗ Калибровка не удалась" << std::endl;
            }
        });

        metrics_collector_ptr->startCalibration(90);

        // Ожидаем завершения калибровки
        std::cout << "Прогресс калибровки: ";
        int dots = 0;
        while (metrics_collector_ptr->isCalibrating() && !g_should_exit) {
            capsule_manager.update();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout << "." << std::flush;
            if (++dots % 10 == 0) {
                std::cout << " " << dots << "с" << std::flush;
            }
        }
        std::cout << std::endl;

        if (!metrics_collector_ptr->isCalibrating()) {
            std::cout << "✓ Калибровка завершена (ждали " << dots << " секунд)" << std::endl;
        }
    }

    // Сохраняем baseline
    std::cout << "\nСоздание baseline метрик..." << std::endl;
    metrics_collector_ptr->saveBaseline();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    metrics_collector_ptr->saveBaseline();

    // Сохраняем baseline в профиль пользователя
    if (db.isOpen()) {
        auto baseline = metrics_collector_ptr->getBaseline();
        user_profile.baseline_alpha = baseline.alpha_power;
        user_profile.baseline_beta = baseline.beta_power;
        user_profile.baseline_theta = baseline.theta_power;
        user_profile.baseline_concentration = baseline.concentration;
        user_profile.baseline_relaxation = baseline.relaxation;
        user_profile.baseline_heart_rate = baseline.heart_rate;
        db.saveUserProfile(user_profile);
        std::cout << "✓ Baseline сохранен в профиль" << std::endl;
    }

    // Создаем библиотеку упражнений
    ExerciseLibrary library;

    // Основной цикл выбора и выполнения упражнений
    while (!g_should_exit) {
        // Показываем меню и получаем выбранное упражнение
        auto exercise = library.showMenu();

        // Если пользователь выбрал выход (nullptr)
        if (!exercise) {
            std::cout << "\nЗавершение работы..." << std::endl;
            break;
        }

        // Настраиваем callbacks для упражнения
        exercise->setOnPhaseChangedCallback([](ExercisePhase phase) {
        std::cout << "\n>>> Фаза изменена: ";
        switch (phase) {
            case ExercisePhase::Preparation:
                std::cout << "ПОДГОТОВКА" << std::endl;
                break;
            case ExercisePhase::Execution:
                std::cout << "ВЫПОЛНЕНИЕ" << std::endl;
                break;
            case ExercisePhase::Completion:
                std::cout << "ЗАВЕРШЕНИЕ" << std::endl;
                break;
            case ExercisePhase::Finished:
                std::cout << "ЗАВЕРШЕНО" << std::endl;
                break;
            default:
                break;
            }
        });

        exercise->setOnNewInstructionCallback([](const Instruction& instruction) {
            std::cout << "\n📝 Инструкция: " << instruction.text << std::endl;
            if (instruction.duration_seconds > 0) {
                std::cout << "   Длительность: " << instruction.duration_seconds << " сек" << std::endl;
            }
        });

        exercise->setOnProgressUpdateCallback([](double progress, double target_score) {
            static int last_progress = -1;
            int current_progress = static_cast<int>(progress);

            if (current_progress != last_progress && current_progress % 10 == 0) {
                std::cout << "\r[" << current_progress << "%] Соответствие цели: "
                          << static_cast<int>(target_score) << "%" << std::flush;
                last_progress = current_progress;
            }
        });

        // Запускаем упражнение
        std::cout << "\n\n═══════════════════════════════════════════════════════════════" << std::endl;
        std::cout << "  Начало упражнения: " << exercise->getName() << std::endl;
        std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;
        std::cout << "\n" << exercise->getDescription() << std::endl;
        std::cout << "\nНажмите Enter для начала..." << std::endl;
        std::string response;
        std::getline(std::cin, response);

        exercise->start(*metrics_collector_ptr);
        session_manager.markActivity(static_cast<clCUserActivity>(0));  // Подготовка

        auto start_time = std::chrono::steady_clock::now();

        // Основной цикл выполнения упражнения
        std::cout << "\nУпражнение запущено. Нажмите Ctrl+C для выхода." << std::endl;

        while (!g_should_exit && exercise->getCurrentPhase() != ExercisePhase::Finished) {
            // Обновляем Capsule client
            capsule_manager.update();

            // Получаем текущие метрики
            auto current_metrics = metrics_collector_ptr->getCurrentMetrics();

            // Обновляем упражнение
            auto now = std::chrono::steady_clock::now();
            double elapsed = std::chrono::duration<double>(now - start_time).count();
            exercise->update(current_metrics, elapsed);

            // Показываем метрики каждые 10 секунд
            static auto last_print = std::chrono::steady_clock::now();
            if (std::chrono::duration<double>(now - last_print).count() >= 10.0) {
                printMetrics(current_metrics);
                last_print = now;
            }

            // Обновляем маркер активности в зависимости от фазы
            static ExercisePhase last_phase = ExercisePhase::NotStarted;
            if (exercise->getCurrentPhase() != last_phase) {
                switch (exercise->getCurrentPhase()) {
                    case ExercisePhase::Preparation:
                        session_manager.markActivity(clCUserActivity1);
                        break;
                    case ExercisePhase::Execution:
                        session_manager.markActivity(clCUserActivity2);
                        break;
                    case ExercisePhase::Completion:
                        session_manager.markActivity(clCUserActivity3);
                        break;
                    default:
                        break;
                }
                last_phase = exercise->getCurrentPhase();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        // Завершаем упражнение
        exercise->finish();

        // Показываем статистику
        auto stats = exercise->getStatistics();
        std::cout << "\n\n═══════════════════════════════════════════════════════════════" << std::endl;
        std::cout << "  Результаты упражнения: " << exercise->getName() << std::endl;
        std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;
        std::cout << "Общее время:               " << static_cast<int>(stats.total_duration) << " сек" << std::endl;
        std::cout << "Время в целевом состоянии: " << static_cast<int>(stats.time_in_target_state) << " сек" << std::endl;
        std::cout << "Средняя оценка:            " << static_cast<int>(stats.average_target_score) << "%" << std::endl;
        std::cout << "Максимальная оценка:       " << static_cast<int>(stats.max_target_score) << "%" << std::endl;
        std::cout << "Итоговый успех:            " << static_cast<int>(stats.success_rate) << "%" << std::endl;

        // Показываем отклонение от baseline
        auto deviation = metrics_collector_ptr->getDeviationFromBaseline();
        std::cout << "\n=== Изменения относительно baseline ===" << std::endl;
        std::cout << "Альфа:        " << std::showpos << deviation.alpha_power << "%" << std::noshowpos << std::endl;
        std::cout << "Бета:         " << std::showpos << deviation.beta_power << "%" << std::noshowpos << std::endl;
        std::cout << "Концентрация: " << std::showpos << deviation.concentration << "%" << std::noshowpos << std::endl;
        std::cout << "Релаксация:   " << std::showpos << deviation.relaxation << "%" << std::noshowpos << std::endl;

        // Сохраняем сессию в БД
        if (db.isOpen()) {
            SessionRecord record;
            record.user_id = "default";  // Пока один пользователь
            record.exercise_name = exercise->getName();
            // timestamp заполнится автоматически в saveSession()
            record.duration_seconds = static_cast<int>(stats.total_duration);
            record.success_rate = stats.success_rate;

            // НОВОЕ: сохраняем информацию о ступени
            record.exercise_stage = static_cast<int>(exercise->getStage());
            record.exercise_order_in_stage = exercise->getOrderInStage();

            // Используем текущие метрики как средние (approximation)
            // TODO: В будущем добавить реальный расчет средних за всю сессию
            auto current_metrics = metrics_collector_ptr->getCurrentMetrics();
            record.avg_alpha = current_metrics.alpha_power;
            record.avg_beta = current_metrics.beta_power;
            record.avg_theta = current_metrics.theta_power;
            record.avg_concentration = current_metrics.concentration;
            record.avg_relaxation = current_metrics.relaxation;
            record.avg_fatigue = current_metrics.fatigue;
            record.avg_focus = current_metrics.focus;
            record.avg_stress = current_metrics.stress;
            record.avg_heart_rate = current_metrics.heart_rate;

            // Путь к CSV файлу
            // NOTE: MetricsCollector не хранит путь к файлу публично, используем дефолтное имя
            // TODO: Добавить метод getLogFilePath() в MetricsCollector
            record.csv_file_path = "";  // Пока пусто

            if (db.saveSession(record)) {
                std::cout << "\n💾 Сессия сохранена в базу данных" << std::endl;

                // Показываем статистику по этому упражнению
                showStatistics(db, exercise->getName());
            } else {
                std::cerr << "\n⚠️  Не удалось сохранить сессию в БД" << std::endl;
            }
        }

        // Спрашиваем, продолжать ли
        std::cout << "\n\n╔═══════════════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║                      Что дальше?                              ║" << std::endl;
        std::cout << "╚═══════════════════════════════════════════════════════════════╝" << std::endl;
        std::cout << "\n  [1] Выбрать другое упражнение" << std::endl;
        std::cout << "  [S] Показать общую статистику" << std::endl;
        std::cout << "  [P] Показать прогресс по ступеням" << std::endl;  // НОВОЕ
        std::cout << "  [0] Выход из программы" << std::endl;
        std::cout << "\nВаш выбор: ";

        std::string continue_choice_str;
        std::getline(std::cin, continue_choice_str);

        // Обработка выбора
        if (continue_choice_str == "0") {
            std::cout << "\nЗавершение работы..." << std::endl;
            break;
        } else if (continue_choice_str == "S" || continue_choice_str == "s") {
            // Показываем общую статистику
            if (db.isOpen()) {
                showUserStats(db);

                // После статистики спрашиваем снова
                std::cout << "\nНажмите Enter для продолжения...";
                std::getline(std::cin, continue_choice_str);
            }
        } else if (continue_choice_str == "P" || continue_choice_str == "p") {  // НОВОЕ
            // Показываем прогресс по ступеням
            if (db.isOpen()) {
                showStageProgress(db);

                // После статистики спрашиваем снова
                std::cout << "\nНажмите Enter для продолжения...";
                std::getline(std::cin, continue_choice_str);
            }
        } else if (continue_choice_str != "1") {
            std::cout << "⚠️  Неверный выбор. Возвращаемся к выбору упражнения..." << std::endl;
        }
        // Если выбор == "1" или неверный ввод, просто продолжаем цикл

        // Сбрасываем baseline для следующего упражнения
        std::cout << "\nОбновление baseline для нового упражнения..." << std::endl;
        metrics_collector_ptr->saveBaseline();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    } // Конец цикла while для выбора упражнений

    // Очистка с правильным ожиданием асинхронных операций
    std::cout << "\nОстановка сессии..." << std::endl;
    session_manager.stopSession();

    // КРИТИЧНО: Даем время CapsuleAPI обработать callback остановки
    std::cout << "Ожидание завершения остановки сессии..." << std::endl;
    for (int i = 0; i < 20; ++i) {  // Ждем до 2 секунд
        capsule_manager.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Теперь безопасно уничтожить сессию
    session_manager.destroySession();

    std::cout << "Отключение от устройства..." << std::endl;
    capsule_manager.disconnectDevice();

    // Еще один update для обработки отключения устройства
    for (int i = 0; i < 10; ++i) {
        capsule_manager.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::cout << "Отключение от Capsule..." << std::endl;
    capsule_manager.disconnect();

    // Финальный update
    for (int i = 0; i < 5; ++i) {
        capsule_manager.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::cout << "\nДанные сохранены в: energy_ball_session.csv" << std::endl;
    std::cout << "Завершение работы. До свидания!" << std::endl;

    return 0;
}
