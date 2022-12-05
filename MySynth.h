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

    void noteOn(int midiChannel, int midiNoteNumber, float velocity) override
    {
        const juce::ScopedLock sl(lock);

        for (auto *sound: sounds)
        {
            if (auto *mySound = dynamic_cast<MySamplerSound *> (sound))
            {
                if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel) && mySound->appliesToVelocity(velocity))
                {
                    DBG("mysound : " << mySound->getName());
                    // If hitting a note that's still ringing, stop it first (it could be
                    // still playing because of the sustain or sostenuto pedal).
                    for (auto *voice: voices)
                        if (voice->getCurrentlyPlayingNote() == midiNoteNumber && voice->isPlayingChannel(midiChannel))
                            stopVoice(voice, 1.0f, true);

                    startVoice(findFreeVoice(sound, midiChannel, midiNoteNumber, true),
                               sound, midiChannel, midiNoteNumber, velocity);
                }
            }
        }

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
                                    0.f,   // min velocity
                                    0.5,   // max velocity
                                    10.0  // maximum sample length
        ));

        auto audioFile2 = assetsDir.getChildFile("oneClick.wav");
        jassert (audioFile2.existsAsFile());
        auto sampleStream2 = audioFile2.createInputStream();
        std::unique_ptr<juce::AudioFormatReader> audioReader2(wavFormat.createReaderFor(sampleStream2.release(), true));

        addSound(new MySamplerSound("kick",
                                    *audioReader2,
                                    allNotes,
                                    0.5,   // min velocity
                                    1.f,   // max velocity
                                    10.0  // maximum sample length
        ));
    }


private:


};

#endif //TK_SAMPLE_PLAYER_MYSYNTH_H
