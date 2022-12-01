#include "MySamplerVoice.h"

#include <memory>

MySamplerSound::MySamplerSound(const juce::String &soundName,
                               juce::AudioFormatReader &source,
                               const juce::BigInteger &notes,
                               int midiNoteForNormalPitch,
                               double maxSampleLengthSeconds)
        : name(soundName),
          sourceSampleRate(source.sampleRate),
          midiNotes(notes),
          midiRootNote(midiNoteForNormalPitch)
{
    if (sourceSampleRate > 0 && source.lengthInSamples > 0)
    {
        length = juce::jmin((int) source.lengthInSamples,
                            (int) (maxSampleLengthSeconds * sourceSampleRate));

        data = std::make_unique<juce::AudioBuffer<float>>(juce::jmin(2, (int) source.numChannels), length + 4);

        source.read(data.get(), 0, length + 4, 0, true, true);
    }
}

bool MySamplerSound::appliesToNote(int midiNoteNumber)
{
    return midiNotes[midiNoteNumber];
}

bool MySamplerSound::appliesToChannel(int /*midiChannel*/)
{
    return true;
}

//===================================================================

bool MySamplerVoice::canPlaySound(juce::SynthesiserSound *sound)
{
    return dynamic_cast<const MySamplerSound *> (sound) != nullptr;
}

void MySamplerVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *s,
                               int /*currentPitchWheelPosition*/)
{

    DBG("start Note ==============>0<===================");
    if (auto *sound = dynamic_cast<const MySamplerSound *> (s))
    {
        sourceSamplePosition = 0.0;
        lgain = velocity;
        rgain = velocity;

    } else
    {
        jassertfalse; // this object can only play SamplerSounds!
    }
}

void MySamplerVoice::stopNote(float /*velocity*/, bool /*allowTailOff*/)
{
    clearCurrentNote();
}

void MySamplerVoice::pitchWheelMoved(int /*newValue*/)
{}

void MySamplerVoice::controllerMoved(int /*controllerNumber*/, int /*newValue*/)
{}

//==============================================================================
void MySamplerVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    if (auto *playingSound = dynamic_cast<MySamplerSound *> (getCurrentlyPlayingSound().get()))
    {
        auto &data = *playingSound->data;
        const float *const inL = data.getReadPointer(0);
        const float *const inR = data.getNumChannels() > 1 ? data.getReadPointer(1) : nullptr;

        float *outL = outputBuffer.getWritePointer(0, startSample);
        float *outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer(1, startSample) : nullptr;

        while (--numSamples >= 0)
        {
            float l = inL[sourceSamplePosition];
            float r = (inR != nullptr) ? inR[sourceSamplePosition] : l;

            l *= lgain;
            r *= rgain;

            if (outR != nullptr)
            {
                *outL++ += l;
                *outR++ += r;
            } else
            {
                *outL++ += (l + r) * 0.5f;
            }
            ++sourceSamplePosition;

            if (sourceSamplePosition > playingSound->length)
            {
                DBG("out of samples" << int(sourceSamplePosition));
                stopNote(0.0f, false);
                break;
            }
        }
    }
}
