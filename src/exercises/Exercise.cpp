#include "Exercise.h"
#include <chrono>
#include <numeric>

namespace Bronnikov {

Exercise::Exercise(const std::string& name, const std::string& description)
    : m_name(name)
    , m_description(description)
{
}

void Exercise::start(MetricsCollector& metrics_collector) {
    m_current_phase = ExercisePhase::Preparation;
    m_current_instruction_index = 0;
    m_phase_start_time = std::chrono::duration<double>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    m_statistics = Statistics();
    m_target_scores.clear();

    if (m_on_phase_changed) {
        m_on_phase_changed(m_current_phase);
    }

    if (m_on_new_instruction && !m_instructions.empty()) {
        m_on_new_instruction(m_instructions[0]);
    }
}

void Exercise::update(const MetricsSnapshot& current_metrics, double elapsed_seconds) {
    m_statistics.total_duration = elapsed_seconds;

    // НОВОЕ: Накопление метрик в зависимости от текущей фазы
    if (m_current_phase == ExercisePhase::Baseline) {
        // Накапливаем baseline метрики
        if (m_phase_metrics.baseline_samples == 0) {
            m_phase_metrics.baseline = current_metrics;
        } else {
            // Усреднение метрик
            auto& b = m_phase_metrics.baseline;
            int n = m_phase_metrics.baseline_samples;
            b.alpha_power = (b.alpha_power * n + current_metrics.alpha_power) / (n + 1);
            b.beta_power = (b.beta_power * n + current_metrics.beta_power) / (n + 1);
            b.theta_power = (b.theta_power * n + current_metrics.theta_power) / (n + 1);
            b.concentration = (b.concentration * n + current_metrics.concentration) / (n + 1);
            b.relaxation = (b.relaxation * n + current_metrics.relaxation) / (n + 1);
            b.fatigue = (b.fatigue * n + current_metrics.fatigue) / (n + 1);
            b.heart_rate = (b.heart_rate * n + current_metrics.heart_rate) / (n + 1);
        }
        m_phase_metrics.baseline_samples++;
    } else if (m_current_phase == ExercisePhase::Execution) {
        // Накапливаем active метрики
        if (m_phase_metrics.active_samples == 0) {
            m_phase_metrics.active = current_metrics;
        } else {
            auto& a = m_phase_metrics.active;
            int n = m_phase_metrics.active_samples;
            a.alpha_power = (a.alpha_power * n + current_metrics.alpha_power) / (n + 1);
            a.beta_power = (a.beta_power * n + current_metrics.beta_power) / (n + 1);
            a.theta_power = (a.theta_power * n + current_metrics.theta_power) / (n + 1);
            a.concentration = (a.concentration * n + current_metrics.concentration) / (n + 1);
            a.relaxation = (a.relaxation * n + current_metrics.relaxation) / (n + 1);
            a.fatigue = (a.fatigue * n + current_metrics.fatigue) / (n + 1);
            a.heart_rate = (a.heart_rate * n + current_metrics.heart_rate) / (n + 1);
        }
        m_phase_metrics.active_samples++;
    } else if (m_current_phase == ExercisePhase::Post) {
        // Накапливаем post метрики
        if (m_phase_metrics.post_samples == 0) {
            m_phase_metrics.post = current_metrics;
        } else {
            auto& p = m_phase_metrics.post;
            int n = m_phase_metrics.post_samples;
            p.alpha_power = (p.alpha_power * n + current_metrics.alpha_power) / (n + 1);
            p.beta_power = (p.beta_power * n + current_metrics.beta_power) / (n + 1);
            p.theta_power = (p.theta_power * n + current_metrics.theta_power) / (n + 1);
            p.concentration = (p.concentration * n + current_metrics.concentration) / (n + 1);
            p.relaxation = (p.relaxation * n + current_metrics.relaxation) / (n + 1);
            p.fatigue = (p.fatigue * n + current_metrics.fatigue) / (n + 1);
            p.heart_rate = (p.heart_rate * n + current_metrics.heart_rate) / (n + 1);
        }
        m_phase_metrics.post_samples++;
    }

    // Вычисляем оценку соответствия целевому состоянию
    double target_score = checkTargetState(current_metrics);
    m_target_scores.push_back(target_score);

    // Обновляем статистику
    if (target_score >= 70.0) {  // Считаем что при 70%+ мы в целевом состоянии
        m_statistics.time_in_target_state += 1.0;  // Предполагаем обновление раз в секунду
    }

    if (target_score > m_statistics.max_target_score) {
        m_statistics.max_target_score = target_score;
    }

    // Проверяем необходимость перехода к следующей инструкции
    if (shouldMoveToNextPhase(elapsed_seconds)) {
        nextPhase();
    }

    // Отправляем callback о прогрессе
    if (m_on_progress_update) {
        double progress = (elapsed_seconds / (m_duration_minutes * 60.0)) * 100.0;
        m_on_progress_update(progress, target_score);
    }
}

void Exercise::finish() {
    m_current_phase = ExercisePhase::Finished;

    // Вычисляем итоговые статистики
    if (!m_target_scores.empty()) {
        m_statistics.average_target_score =
            std::accumulate(m_target_scores.begin(), m_target_scores.end(), 0.0) /
            m_target_scores.size();
    }

    m_statistics.success_rate = calculateSuccessRate();

    if (m_on_phase_changed) {
        m_on_phase_changed(m_current_phase);
    }
}

double Exercise::checkTargetState(const MetricsSnapshot& metrics) const {
    // Базовая реализация - переопределяется в подклассах
    return 50.0;
}

double Exercise::calculateSuccessRate() const {
    if (m_statistics.total_duration == 0.0) {
        return 0.0;
    }

    // Успех = комбинация времени в целевом состоянии и средней оценки
    double time_ratio = m_statistics.time_in_target_state / m_statistics.total_duration;
    double score_ratio = m_statistics.average_target_score / 100.0;

    // 60% веса - время в целевом состоянии, 40% - средняя оценка
    return (time_ratio * 60.0 + score_ratio * 40.0);
}

const Instruction* Exercise::getCurrentInstruction() const {
    if (m_current_instruction_index < m_instructions.size()) {
        return &m_instructions[m_current_instruction_index];
    }
    return nullptr;
}

void Exercise::nextPhase() {
    m_current_instruction_index++;

    if (m_current_instruction_index >= m_instructions.size()) {
        // Все инструкции выполнены
        m_current_phase = ExercisePhase::Completion;
        if (m_on_phase_changed) {
            m_on_phase_changed(m_current_phase);
        }
        return;
    }

    const Instruction& next_instruction = m_instructions[m_current_instruction_index];

    // Обновляем фазу если она изменилась
    if (next_instruction.phase != m_current_phase) {
        m_current_phase = next_instruction.phase;
        m_phase_start_time = std::chrono::duration<double>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();

        if (m_on_phase_changed) {
            m_on_phase_changed(m_current_phase);
        }
    }

    // Уведомляем о новой инструкции
    if (m_on_new_instruction) {
        m_on_new_instruction(next_instruction);
    }
}

bool Exercise::shouldMoveToNextPhase(double elapsed_seconds) const {
    const Instruction* current = getCurrentInstruction();
    if (!current) {
        return false;
    }

    if (current->duration_seconds == 0) {
        return false;  // Бесконечная длительность, переход только вручную
    }

    double current_time = std::chrono::duration<double>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    return (current_time - m_phase_start_time) >= current->duration_seconds;
}

// НОВЫЕ МЕТОДЫ: Трехфазный сбор метрик

void Exercise::startBaseline() {
    m_current_phase = ExercisePhase::Baseline;
    m_phase_start_time = std::chrono::duration<double>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    // Сброс baseline метрик
    m_phase_metrics.baseline = MetricsSnapshot();
    m_phase_metrics.baseline_samples = 0;
    m_phase_metrics.has_baseline = false;

    if (m_on_phase_changed) {
        m_on_phase_changed(m_current_phase);
    }
}

void Exercise::startActive() {
    // Фиксируем что baseline собран
    if (m_phase_metrics.baseline_samples > 0) {
        m_phase_metrics.has_baseline = true;
    }

    m_current_phase = ExercisePhase::Execution;
    m_phase_start_time = std::chrono::duration<double>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    // Сброс active метрик
    m_phase_metrics.active = MetricsSnapshot();
    m_phase_metrics.active_samples = 0;
    m_phase_metrics.has_active = false;

    if (m_on_phase_changed) {
        m_on_phase_changed(m_current_phase);
    }
}

void Exercise::startPost() {
    // Фиксируем что active собран
    if (m_phase_metrics.active_samples > 0) {
        m_phase_metrics.has_active = true;
    }

    m_current_phase = ExercisePhase::Post;
    m_phase_start_time = std::chrono::duration<double>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    // Сброс post метрик
    m_phase_metrics.post = MetricsSnapshot();
    m_phase_metrics.post_samples = 0;
    m_phase_metrics.has_post = false;

    if (m_on_phase_changed) {
        m_on_phase_changed(m_current_phase);
    }
}

ComparisonResult Exercise::comparePhases() const {
    ComparisonResult result;

    // Фиксируем что post собран
    if (m_phase_metrics.post_samples > 0) {
        const_cast<PhaseMetrics&>(m_phase_metrics).has_post = true;
    }

    if (!m_phase_metrics.has_baseline) {
        // Нет baseline данных - невозможно сравнить
        return result;
    }

    const auto& baseline = m_phase_metrics.baseline;
    const auto& active = m_phase_metrics.active;
    const auto& post = m_phase_metrics.post;

    // Вспомогательная функция для расчета процентного изменения
    auto calcChange = [](double current, double baseline_val) -> double {
        if (baseline_val == 0.0) return 0.0;
        return ((current - baseline_val) / baseline_val) * 100.0;
    };

    // Изменения Alpha
    if (m_phase_metrics.has_active) {
        result.alpha_active_change = calcChange(active.alpha_power, baseline.alpha_power);
    }
    if (m_phase_metrics.has_post) {
        result.alpha_change = calcChange(post.alpha_power, baseline.alpha_power);
    }

    // Изменения Beta
    if (m_phase_metrics.has_active) {
        result.beta_active_change = calcChange(active.beta_power, baseline.beta_power);
    }
    if (m_phase_metrics.has_post) {
        result.beta_change = calcChange(post.beta_power, baseline.beta_power);
    }

    // Изменения Theta
    if (m_phase_metrics.has_active) {
        result.theta_active_change = calcChange(active.theta_power, baseline.theta_power);
    }
    if (m_phase_metrics.has_post) {
        result.theta_change = calcChange(post.theta_power, baseline.theta_power);
    }

    // Изменения Concentration
    if (m_phase_metrics.has_active) {
        result.concentration_active_change = calcChange(active.concentration, baseline.concentration);
    }
    if (m_phase_metrics.has_post) {
        result.concentration_change = calcChange(post.concentration, baseline.concentration);
    }

    // Изменения Relaxation
    if (m_phase_metrics.has_active) {
        result.relaxation_active_change = calcChange(active.relaxation, baseline.relaxation);
    }
    if (m_phase_metrics.has_post) {
        result.relaxation_change = calcChange(post.relaxation, baseline.relaxation);
    }

    // Изменения Fatigue
    if (m_phase_metrics.has_active) {
        result.fatigue_active_change = calcChange(active.fatigue, baseline.fatigue);
    }
    if (m_phase_metrics.has_post) {
        result.fatigue_change = calcChange(post.fatigue, baseline.fatigue);
    }

    // Оценка эффективности упражнения
    // Проверяем достижение целевых изменений из m_target_metrics
    double effectiveness_score = 0.0;
    double total_weight = 0.0;

    // Alpha
    if (m_phase_metrics.has_post && m_target_metrics.alpha_weight > 0.0) {
        double target_change = m_target_metrics.alpha_change;
        double actual_change = result.alpha_change;
        double match = 1.0 - std::abs(target_change - actual_change) / std::max(std::abs(target_change), 1.0);
        match = std::max(0.0, match);
        effectiveness_score += match * m_target_metrics.alpha_weight;
        total_weight += m_target_metrics.alpha_weight;
    }

    // Beta
    if (m_phase_metrics.has_post && m_target_metrics.beta_weight > 0.0) {
        double target_change = m_target_metrics.beta_change;
        double actual_change = result.beta_change;
        double match = 1.0 - std::abs(target_change - actual_change) / std::max(std::abs(target_change), 1.0);
        match = std::max(0.0, match);
        effectiveness_score += match * m_target_metrics.beta_weight;
        total_weight += m_target_metrics.beta_weight;
    }

    // Theta
    if (m_phase_metrics.has_post && m_target_metrics.theta_weight > 0.0) {
        double target_change = m_target_metrics.theta_change;
        double actual_change = result.theta_change;
        double match = 1.0 - std::abs(target_change - actual_change) / std::max(std::abs(target_change), 1.0);
        match = std::max(0.0, match);
        effectiveness_score += match * m_target_metrics.theta_weight;
        total_weight += m_target_metrics.theta_weight;
    }

    // Concentration
    if (m_phase_metrics.has_post && m_target_metrics.concentration_weight > 0.0) {
        double target_change = m_target_metrics.concentration_change;
        double actual_change = result.concentration_change;
        double match = 1.0 - std::abs(target_change - actual_change) / std::max(std::abs(target_change), 1.0);
        match = std::max(0.0, match);
        effectiveness_score += match * m_target_metrics.concentration_weight;
        total_weight += m_target_metrics.concentration_weight;
    }

    // Relaxation
    if (m_phase_metrics.has_post && m_target_metrics.relaxation_weight > 0.0) {
        double target_change = m_target_metrics.relaxation_change;
        double actual_change = result.relaxation_change;
        double match = 1.0 - std::abs(target_change - actual_change) / std::max(std::abs(target_change), 1.0);
        match = std::max(0.0, match);
        effectiveness_score += match * m_target_metrics.relaxation_weight;
        total_weight += m_target_metrics.relaxation_weight;
    }

    // Итоговая эффективность
    if (total_weight > 0.0) {
        result.overall_effectiveness = (effectiveness_score / total_weight) * 100.0;
        result.target_achieved = (result.overall_effectiveness >= 70.0);
    }

    return result;
}

} // namespace Bronnikov
