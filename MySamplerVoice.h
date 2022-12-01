#ifndef TK_SAMPLE_PLAYER_MYSAMPLERVOICE_H
#define TK_SAMPLE_PLAYER_MYSAMPLERVOICE_H

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_audio_processors/juce_audio_processors.h>

class MySamplerSound : public juce::SynthesiserSound
{
public:
    MySamplerSound(const juce::String &name,
                   juce::AudioFormatReader &source,
                   const juce::BigInteger &midiNotes,
                   int midiNoteForNormalPitch,
                   double maxSampleLengthSeconds);

    ~MySamplerSound() override = default;
    //==============================================================================
    /** Returns the sample's name */
    const juce::String &getName() const noexcept
    { return name; }

    /** Returns the audio sample data.
        This could return nullptr if there was a problem loading the data.
    */
    juce::AudioBuffer<float> *getAudioData() const noexcept
    { return data.get(); }

    //==============================================================================
    bool appliesToNote(int midiNoteNumber) override;

    bool appliesToChannel(int midiChannel) override;

private:
    //==============================================================================
    friend class MySamplerVoice;

    juce::String name;
    std::unique_ptr<juce::AudioBuffer<float>> data;
    double sourceSampleRate;
    juce::BigInteger midiNotes;
    int length = 0, midiRootNote = 0;


    JUCE_LEAK_DETECTOR (MySamplerSound)
};

class MySamplerVoice : public juce::SynthesiserVoice
{
public:
//==============================================================================
/** Creates a SamplerVoice. */
    MySamplerVoice()=default;

/** Destructor. */
    ~MySamplerVoice() override = default;

//==============================================================================
    bool canPlaySound(juce::SynthesiserSound *) override;

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *, int pitchWheel) override;

    void stopNote(float velocity, bool allowTailOff) override;

    void pitchWheelMoved(int newValue) override;

    void controllerMoved(int controllerNumber, int newValue) override;

    void renderNextBlock(juce::AudioBuffer<float> &, int startSample, int numSamples) override;

    using SynthesiserVoice::renderNextBlock;


private:
    uint32_t sourceSamplePosition = 0;
    float lgain = 0, rgain = 0;

    JUCE_LEAK_DETECTOR (MySamplerVoice)
};

#endif //TK_SAMPLE_PLAYER_MYSAMPLERVOICE_H
