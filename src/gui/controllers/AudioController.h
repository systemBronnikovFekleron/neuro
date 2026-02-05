#ifndef AUDIOCONTROLLER_H
#define AUDIOCONTROLLER_H

#include <QObject>
#include <QTextToSpeech>
#include <QSoundEffect>
#include <QString>
#include <memory>

class SettingsController;

/**
 * @brief Контроллер для управления аудио и голосовой обратной связью
 *
 * Управляет:
 * - Text-to-Speech для голосовых инструкций
 * - Звуковыми эффектами для UI событий
 * - Интеграцией с настройками пользователя
 */
class AudioController : public QObject {
    Q_OBJECT

    // Properties для QML
    Q_PROPERTY(bool isSpeaking READ isSpeaking NOTIFY isSpeakingChanged)
    Q_PROPERTY(QString currentVoice READ currentVoice NOTIFY currentVoiceChanged)
    Q_PROPERTY(double volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(double rate READ rate WRITE setRate NOTIFY rateChanged)

public:
    explicit AudioController(SettingsController* settingsController, QObject* parent = nullptr);
    ~AudioController() override;

    // Getters
    bool isSpeaking() const;
    QString currentVoice() const;
    double volume() const;
    double rate() const;

    // Setters
    void setVolume(double volume);
    void setRate(double rate);

public slots:
    // TTS методы
    void speakInstruction(const QString& text);
    void stopSpeaking();

    // Звуковые эффекты
    void playPhaseChangeSound();
    void playSuccessSound();
    void playExerciseStartSound();
    void playExerciseCompleteSound();

    // Реакции на события упражнения
    void onPhaseChanged(const QString& phaseName);
    void onInstructionChanged(const QString& instruction);
    void onExerciseStarted(const QString& exerciseName);
    void onExerciseCompleted(double successRate);
    void onTargetStateAchieved();

signals:
    void isSpeakingChanged();
    void currentVoiceChanged();
    void volumeChanged();
    void rateChanged();
    void ttsError(const QString& error);

private:
    void initializeTTS();
    void initializeSoundEffects();
    void updateTTSSettings();
    bool shouldPlaySound() const;
    bool shouldSpeak() const;

    std::unique_ptr<QTextToSpeech> m_tts;
    std::unique_ptr<QSoundEffect> m_phaseChangeSound;
    std::unique_ptr<QSoundEffect> m_successSound;
    std::unique_ptr<QSoundEffect> m_exerciseStartSound;
    std::unique_ptr<QSoundEffect> m_exerciseCompleteSound;

    SettingsController* m_settingsController;
    double m_volume = 0.7;  // 70% по умолчанию
    double m_rate = 1.0;    // Нормальная скорость
};

#endif // AUDIOCONTROLLER_H
