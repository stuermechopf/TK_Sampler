#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p)
        : AudioProcessorEditor(&p),
          processorRef(p),
          midiKeyboard(p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible (midiKeyboard);

    processorRef.keyboardState.addListener(this);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(500, 300);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    processorRef.keyboardState.removeListener(this);
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Hello Tubekillers!", getLocalBounds(), juce::Justification::centred, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    auto r = getLocalBounds().reduced (8);

    //r.removeFromTop (26);
    midiKeyboard        .setBounds (r.removeFromBottom (70));

}

void AudioPluginAudioProcessorEditor::timerCallback()
{
// update note on
}

void AudioPluginAudioProcessorEditor::handleNoteOn(juce::MidiKeyboardState *source, int midiChannel, int midiNoteNumber,
                                                   float velocity)
{
    juce::ignoreUnused(source, midiChannel, midiNoteNumber, velocity);
    DBG("Note on===");
    noteActive=true;
}

void AudioPluginAudioProcessorEditor::handleNoteOff(juce::MidiKeyboardState *source, int midiChannel, int midiNoteNumber,
                                               float velocity)
{
    juce::ignoreUnused(source, midiChannel, midiNoteNumber, velocity);
    DBG("Note off===");
    noteActive=false;
}
