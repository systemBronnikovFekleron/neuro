#include "AudioController.h"
#include "SettingsController.h"
#include <QDebug>
#include <QLocale>

AudioController::AudioController(SettingsController* settingsController, QObject* parent)
    : QObject(parent)
    , m_settingsController(settingsController)
{
    initializeTTS();
    initializeSoundEffects();

    qDebug() << "[AudioController] Initialized with volume:" << m_volume << "rate:" << m_rate;
}

AudioController::~AudioController() {
    if (m_tts && m_tts->state() == QTextToSpeech::Speaking) {
        m_tts->stop();
    }
}

// ============================================================================
// TTS Initialization
// ============================================================================

void AudioController::initializeTTS() {
    m_tts = std::make_unique<QTextToSpeech>(this);

    // Выбрать русский голос, если доступен
    QVector<QVoice> voices = m_tts->availableVoices();
    bool foundRussian = false;

    for (const QVoice& voice : voices) {
        if (voice.locale().language() == QLocale::Russian) {
            m_tts->setVoice(voice);
            qDebug() << "[AudioController] Russian voice found:" << voice.name()
                     << "Gender:" << voice.gender() << "Age:" << voice.age();
            foundRussian = true;
            break;
        }
    }

    if (!foundRussian) {
        qWarning() << "[AudioController] No Russian voice found, using default";
        if (!voices.isEmpty()) {
            m_tts->setVoice(voices.first());
            qDebug() << "[AudioController] Using voice:" << voices.first().name();
        }
    }

    // Установить начальные параметры
    m_tts->setVolume(m_volume);
    m_tts->setRate(m_rate);

    // Подключить сигналы
    connect(m_tts.get(), &QTextToSpeech::stateChanged, this, [this](QTextToSpeech::State state) {
        emit isSpeakingChanged();
        if (state == QTextToSpeech::Error) {
            qCritical() << "[AudioController] TTS Error occurred";
            emit ttsError("TTS Error occurred");
        } else if (state == QTextToSpeech::Speaking) {
            qDebug() << "[AudioController] TTS started speaking";
        } else if (state == QTextToSpeech::Ready) {
            qDebug() << "[AudioController] TTS finished speaking";
        }
    });

    emit currentVoiceChanged();
}

// ============================================================================
// Sound Effects Initialization
// ============================================================================

void AudioController::initializeSoundEffects() {
    // Phase change sound
    m_phaseChangeSound = std::make_unique<QSoundEffect>(this);
    m_phaseChangeSound->setSource(QUrl("qrc:/sounds/phase_change.wav"));
    m_phaseChangeSound->setVolume(m_volume);

    connect(m_phaseChangeSound.get(), &QSoundEffect::statusChanged, this, [this]() {
        if (m_phaseChangeSound->status() == QSoundEffect::Error) {
            qWarning() << "[AudioController] Phase change sound error";
        }
    });

    // Success sound
    m_successSound = std::make_unique<QSoundEffect>(this);
    m_successSound->setSource(QUrl("qrc:/sounds/success.wav"));
    m_successSound->setVolume(m_volume);

    connect(m_successSound.get(), &QSoundEffect::statusChanged, this, [this]() {
        if (m_successSound->status() == QSoundEffect::Error) {
            qWarning() << "[AudioController] Success sound error";
        }
    });

    // Exercise start sound
    m_exerciseStartSound = std::make_unique<QSoundEffect>(this);
    m_exerciseStartSound->setSource(QUrl("qrc:/sounds/exercise_start.wav"));
    m_exerciseStartSound->setVolume(m_volume);

    connect(m_exerciseStartSound.get(), &QSoundEffect::statusChanged, this, [this]() {
        if (m_exerciseStartSound->status() == QSoundEffect::Error) {
            qWarning() << "[AudioController] Exercise start sound error";
        }
    });

    // Exercise complete sound
    m_exerciseCompleteSound = std::make_unique<QSoundEffect>(this);
    m_exerciseCompleteSound->setSource(QUrl("qrc:/sounds/exercise_complete.wav"));
    m_exerciseCompleteSound->setVolume(m_volume);

    connect(m_exerciseCompleteSound.get(), &QSoundEffect::statusChanged, this, [this]() {
        if (m_exerciseCompleteSound->status() == QSoundEffect::Error) {
            qWarning() << "[AudioController] Exercise complete sound error";
        }
    });

    qDebug() << "[AudioController] Sound effects initialized";
}

// ============================================================================
// Getters
// ============================================================================

bool AudioController::isSpeaking() const {
    return m_tts && m_tts->state() == QTextToSpeech::Speaking;
}

QString AudioController::currentVoice() const {
    if (m_tts) {
        QVoice voice = m_tts->voice();
        return voice.name();
    }
    return QString();
}

double AudioController::volume() const {
    return m_volume;
}

double AudioController::rate() const {
    return m_rate;
}

// ============================================================================
// Setters
// ============================================================================

void AudioController::setVolume(double volume) {
    if (qAbs(m_volume - volume) > 0.01) {
        m_volume = qBound(0.0, volume, 1.0);

        // Update TTS volume
        if (m_tts) {
            m_tts->setVolume(m_volume);
        }

        // Update sound effects volume
        if (m_phaseChangeSound) m_phaseChangeSound->setVolume(m_volume);
        if (m_successSound) m_successSound->setVolume(m_volume);
        if (m_exerciseStartSound) m_exerciseStartSound->setVolume(m_volume);
        if (m_exerciseCompleteSound) m_exerciseCompleteSound->setVolume(m_volume);

        emit volumeChanged();
        qDebug() << "[AudioController] Volume changed to:" << m_volume;
    }
}

void AudioController::setRate(double rate) {
    if (qAbs(m_rate - rate) > 0.01) {
        m_rate = qBound(0.5, rate, 2.0);

        if (m_tts) {
            m_tts->setRate(m_rate);
        }

        emit rateChanged();
        qDebug() << "[AudioController] Speech rate changed to:" << m_rate;
    }
}

// ============================================================================
// TTS Methods
// ============================================================================

void AudioController::speakInstruction(const QString& text) {
    if (!shouldSpeak() || text.isEmpty()) {
        return;
    }

    // Остановить текущую речь, если есть
    if (m_tts->state() == QTextToSpeech::Speaking) {
        m_tts->stop();
    }

    qDebug() << "[AudioController] Speaking:" << text.left(50) << "...";
    m_tts->say(text);
}

void AudioController::stopSpeaking() {
    if (m_tts && m_tts->state() == QTextToSpeech::Speaking) {
        m_tts->stop();
        qDebug() << "[AudioController] Speech stopped";
    }
}

// ============================================================================
// Sound Effect Methods
// ============================================================================

void AudioController::playPhaseChangeSound() {
    if (!shouldPlaySound()) {
        return;
    }

    if (m_phaseChangeSound) {
        if (m_phaseChangeSound->status() == QSoundEffect::Ready) {
            qDebug() << "[AudioController] Playing phase change sound";
            m_phaseChangeSound->play();
        } else {
            qDebug() << "[AudioController] Phase change sound not ready, status:" << m_phaseChangeSound->status();
        }
    }
}

void AudioController::playSuccessSound() {
    if (!shouldPlaySound()) {
        return;
    }

    if (m_successSound) {
        if (m_successSound->status() == QSoundEffect::Ready) {
            qDebug() << "[AudioController] Playing success sound";
            m_successSound->play();
        } else {
            qDebug() << "[AudioController] Success sound not ready, status:" << m_successSound->status();
        }
    }
}

void AudioController::playExerciseStartSound() {
    if (!shouldPlaySound()) {
        return;
    }

    if (m_exerciseStartSound) {
        if (m_exerciseStartSound->status() == QSoundEffect::Ready) {
            qDebug() << "[AudioController] Playing exercise start sound";
            m_exerciseStartSound->play();
        } else {
            qDebug() << "[AudioController] Exercise start sound not ready, status:" << m_exerciseStartSound->status();
        }
    }
}

void AudioController::playExerciseCompleteSound() {
    if (!shouldPlaySound()) {
        return;
    }

    if (m_exerciseCompleteSound) {
        if (m_exerciseCompleteSound->status() == QSoundEffect::Ready) {
            qDebug() << "[AudioController] Playing exercise complete sound";
            m_exerciseCompleteSound->play();
        } else {
            qDebug() << "[AudioController] Exercise complete sound not ready, status:" << m_exerciseCompleteSound->status();
        }
    }
}

// ============================================================================
// Exercise Event Handlers
// ============================================================================

void AudioController::onPhaseChanged(const QString& phaseName) {
    qDebug() << "[AudioController] Phase changed to:" << phaseName;

    playPhaseChangeSound();

    // Озвучить название фазы
    QString announcement;
    if (phaseName == "Базовое состояние") {
        announcement = "Начинается фаза базового состояния. Расслабьтесь и дышите спокойно.";
    } else if (phaseName == "Практика") {
        announcement = "Начинается фаза практики. Следуйте инструкциям.";
    } else if (phaseName == "Завершение") {
        announcement = "Завершающая фаза. Возвращайтесь к обычному состоянию.";
    } else {
        announcement = QString("Начинается фаза: %1").arg(phaseName);
    }

    speakInstruction(announcement);
}

void AudioController::onInstructionChanged(const QString& instruction) {
    qDebug() << "[AudioController] New instruction:" << instruction.left(50) << "...";
    speakInstruction(instruction);
}

void AudioController::onExerciseStarted(const QString& exerciseName) {
    qDebug() << "[AudioController] Exercise started:" << exerciseName;

    playExerciseStartSound();

    QString announcement = QString("Начинаем упражнение: %1").arg(exerciseName);
    speakInstruction(announcement);
}

void AudioController::onExerciseCompleted(double successRate) {
    qDebug() << "[AudioController] Exercise completed. Success rate:" << successRate;

    playExerciseCompleteSound();

    QString announcement;
    if (successRate >= 70.0) {
        announcement = QString("Упражнение завершено успешно. Результат: %1 процентов. Отличная работа!")
            .arg(QString::number(successRate, 'f', 0));
    } else if (successRate >= 50.0) {
        announcement = QString("Упражнение завершено. Результат: %1 процентов. Хороший результат.")
            .arg(QString::number(successRate, 'f', 0));
    } else {
        announcement = QString("Упражнение завершено. Результат: %1 процентов. Продолжайте практиковаться.")
            .arg(QString::number(successRate, 'f', 0));
    }

    speakInstruction(announcement);
}

void AudioController::onTargetStateAchieved() {
    qDebug() << "[AudioController] Target state achieved!";

    playSuccessSound();

    QString announcement = "Отлично! Целевое состояние достигнуто. Продолжайте в том же духе.";
    speakInstruction(announcement);
}

// ============================================================================
// Private Helper Methods
// ============================================================================

bool AudioController::shouldPlaySound() const {
    return m_settingsController && m_settingsController->soundSignals();
}

bool AudioController::shouldSpeak() const {
    return m_settingsController && m_settingsController->voiceInstructions();
}
