#pragma once

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "MySynthVoice.h"

//#include "SampleAudioSource.h"

//==============================================================================
class AudioPluginAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();

    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;

    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;

    bool producesMidi() const override;

    bool isMidiEffect() const override;

    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;

    int getCurrentProgram() override;

    void setCurrentProgram(int index) override;

    const juce::String getProgramName(int index) override;

    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;

    void setStateInformation(const void *data, int sizeInBytes) override;

    juce::MidiKeyboardState keyboardState;

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

        synth.clearSounds();
        synth.addSound(new juce::SamplerSound("demo sound",
                                              *audioReader,
                                              allNotes,
                                              74,   // root midi note
                                              0.1,  // attack time
                                              0.1,  // release time
                                              10.0  // maximum sample length
        ));
    }

private:
    juce::Synthesiser synth;

    void initialiseSynth();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};
