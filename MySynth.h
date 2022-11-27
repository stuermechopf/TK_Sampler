#ifndef TK_SAMPLE_PLAYER_MYSYNTH_H
#define TK_SAMPLE_PLAYER_MYSYNTH_H

#include "MySamplerVoice.h"

class MySynth : public juce::Synthesiser
{
public:
    MySynth()
    {
        auto numVoices = 4;

        for (auto i = 0; i < numVoices; ++i)
        {
            addVoice(new MySamplerVoice());
        }

        setUsingSampledSound();
    }

    void setUsingSampledSound()
    {
        juce::WavAudioFormat wavFormat;

        auto assetsDir = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("JUCE_Assets");
        auto audioFile = assetsDir.getChildFile("cello.wav");
        jassert (audioFile.existsAsFile());
        auto sampleStream = audioFile.createInputStream();
        DBG(assetsDir.getFullPathName());
        std::unique_ptr<juce::AudioFormatReader> audioReader(wavFormat.createReaderFor(sampleStream.release(), true));

        juce::BigInteger allNotes;
        allNotes.setRange(0, 128, true);

        clearSounds();
        addSound(new MySamplerSound("snare",
                                        *audioReader,
                                        allNotes,
                                        74,   // root midi note
                                        10.0  // maximum sample length
        ));
    }

private:


};

#endif //TK_SAMPLE_PLAYER_MYSYNTH_H
