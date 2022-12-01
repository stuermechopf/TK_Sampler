#pragma once

#include "PluginProcessor.h"
#include <juce_gui_extra/juce_gui_extra.h>
//==============================================================================
class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor,
                                        private juce::MidiKeyboardStateListener,
                                        private juce::Timer
{
public:
    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &);

    ~AudioPluginAudioProcessorEditor() override;

//==============================================================================
    void paint(juce::Graphics &) override;

    void resized() override;

    void timerCallback() override;

    void handleNoteOn(juce::MidiKeyboardState *source,
                      int midiChannel, int midiNoteNumber, float velocity) override;

    void handleNoteOff(juce::MidiKeyboardState *source,
                       int midiChannel, int midiNoteNumber, float velocity) override;


private:
// This reference is provided as a quick way for your editor to
// access the processor object that created it.
    AudioPluginAudioProcessor &processorRef;
    juce::MidiKeyboardComponent midiKeyboard;

    std::atomic<bool> noteActive{false};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
