#ifndef TK_SAMPLE_PLAYER_MYSYNTH_H
#define TK_SAMPLE_PLAYER_MYSYNTH_H

class MySynth : public juce::Synthesiser
{
public:
    MySynth()
    {
        auto numVoices = 8;

        // Add some voices...
        for (auto i = 0; i < numVoices; ++i)
        {
            addVoice(new MySineWaveVoice());
            addVoice(new juce::SamplerVoice());
            //addVoice(new MySamplerVoice());    // and these ones play the sampled sounds

        }
        setUsingSineWaveSound();
    }

    void setUsingSineWaveSound()
    {
        addSound (new MySineWaveSound());
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
        addSound(new juce::SamplerSound("demo sound",
                                        *audioReader,
                                        allNotes,
                                        74,   // root midi note
                                        0.1,  // attack time
                                        0.1,  // release time
                                        10.0  // maximum sample length
        ));
    }

private:


};

#endif //TK_SAMPLE_PLAYER_MYSYNTH_H
