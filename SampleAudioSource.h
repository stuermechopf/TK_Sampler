#ifndef TK_SAMPLE_PLAYER_SAMPLEAUDIOSOURCE_H
#define TK_SAMPLE_PLAYER_SAMPLEAUDIOSOURCE_H

#include "juce_audio_devices/midi_io/juce_MidiDevices.h"
#include "juce_audio_devices/midi_io/juce_MidiMessageCollector.h"
#include "juce_audio_formats/juce_audio_formats.h"


inline std::unique_ptr<juce::InputStream> createAssetInputStream(const char *resourcePath)
{
    juce::File assetsDir{"Samples"};
    auto resourceFile = assetsDir.getChildFile(resourcePath);
    jassert (resourceFile.existsAsFile());

    return resourceFile.createInputStream();
}

//==============================================================================
// This is an audio source that streams the output of our demo synth.
struct SynthAudioSource : public juce::AudioSource
{
    SynthAudioSource(juce::MidiKeyboardState &keyState) : keyboardState(keyState)
    {
        // Add some voices to our synth, to play the sounds..
        for (auto i = 0; i < 4; ++i)
        {
            synth.addVoice(new juce::SamplerVoice());    // and these ones play the sampled sounds
        }

        setUsingSampledSound();
    }

    void setUsingSampledSound()
    {
        juce::WavAudioFormat wavFormat;

        std::unique_ptr<juce::AudioFormatReader> audioReader(
                wavFormat.createReaderFor(createAssetInputStream("cello.wav").release(), true));

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

    void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        midiCollector.reset(sampleRate);

        synth.setCurrentPlaybackSampleRate(sampleRate);
    }

    void releaseResources() override
    {}

    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override
    {
        // the synth always adds its output to the audio buffer, so we have to clear it
        // first..
        bufferToFill.clearActiveBufferRegion();

        // fill a midi buffer with incoming messages from the midi input.
        juce::MidiBuffer incomingMidi;
        midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);

        // pass these messages to the keyboard state so that it can update the component
        // to show on-screen which keys are being pressed on the physical midi keyboard.
        // This call will also add midi messages to the buffer which were generated by
        // the mouse-clicking on the on-screen keyboard.
        keyboardState.processNextMidiBuffer(incomingMidi, 0, bufferToFill.numSamples, true);

        // and now get the synth to process the midi events and generate its output.
        synth.renderNextBlock(*bufferToFill.buffer, incomingMidi, 0, bufferToFill.numSamples);
    }

    //==============================================================================
    // this collects real-time midi messages from the midi input device, and
    // turns them into blocks that we can process in our audio callback
    juce::MidiMessageCollector midiCollector;

    // this represents the state of which keys on our on-screen keyboard are held
    // down. When the mouse is clicked on the keyboard component, this object also
    // generates midi messages for this, which we can pass on to our synth.
    juce::MidiKeyboardState &keyboardState;

    // the synth itself!
    juce::Synthesiser synth;
};

#endif //TK_SAMPLE_PLAYER_SAMPLEAUDIOSOURCE_H
