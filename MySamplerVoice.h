#ifndef TK_SAMPLE_PLAYER_MYSAMPLERVOICE_H
#define TK_SAMPLE_PLAYER_MYSAMPLERVOICE_H

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_audio_processors/juce_audio_processors.h>


class MySamplerVoice    : public juce::SynthesiserVoice
{
public:
//==============================================================================
/** Creates a SamplerVoice. */
    MySamplerVoice();

/** Destructor. */
~MySamplerVoice() override;

//==============================================================================
bool canPlaySound (juce::SynthesiserSound*) override;

void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound*, int pitchWheel) override;
void stopNote (float velocity, bool allowTailOff) override;

void pitchWheelMoved (int newValue) override;
void controllerMoved (int controllerNumber, int newValue) override;

void renderNextBlock (juce::AudioBuffer<float>&, int startSample, int numSamples) override;
using SynthesiserVoice::renderNextBlock;

private:
//==============================================================================
double pitchRatio = 0;
double sourceSamplePosition = 0;
float lgain = 0, rgain = 0;

juce::ADSR adsr;

JUCE_LEAK_DETECTOR (MySamplerVoice)
};

#endif //TK_SAMPLE_PLAYER_MYSAMPLERVOICE_H
