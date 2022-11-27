#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p)
        : AudioProcessorEditor(&p),
          processorRef(p),
          midiKeyboard(p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    juce::ignoreUnused(processorRef);


    addAndMakeVisible (sineButton);
    sineButton.setRadioGroupId (321);
    sineButton.setToggleState (true, juce::dontSendNotification);
    sineButton.onClick = [this] { processorRef.setUsingSineWaveSound(); };

    addAndMakeVisible (sampledButton);
    sampledButton.setRadioGroupId (321);
    sampledButton.onClick = [this] { processorRef.setUsingSampledSound(); };

    addAndMakeVisible (midiKeyboard);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(500, 300);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
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

    sineButton          .setBounds (16, 76, 150, 24);
    sampledButton       .setBounds (16, 100, 150, 24);
    auto r = getLocalBounds().reduced (8);

    //r.removeFromTop (26);
    midiKeyboard        .setBounds (r.removeFromBottom (70));

}
