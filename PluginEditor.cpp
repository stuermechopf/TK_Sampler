#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p)
        : AudioProcessorEditor(&p),
          processorRef(p),
          midiKeyboard(p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible(midiKeyboard);

    processorRef.keyboardState.addListener(this);

    lowVelocity.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
    addAndMakeVisible(lowVelocity);
    highVelocity.setColour(juce::TextButton::buttonOnColourId, juce::Colours::red);
    addAndMakeVisible(highVelocity);

    startTimerHz(50);

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
    auto r = getLocalBounds().reduced(8);

    highVelocity.setBounds(r.removeFromTop(50));
    lowVelocity.setBounds(r.removeFromTop(50));

    //r.removeFromTop (26);
    midiKeyboard.setBounds(r.removeFromBottom(70));

}

void AudioPluginAudioProcessorEditor::timerCallback()
{
    if (noteActive)
    {
        if (currentVelocity > 0.5)
        {
            highVelocity.setToggleState(true, juce::NotificationType::dontSendNotification);
            highVelocity.setButtonText(juce::String(currentVelocity));
        } else
        {
            lowVelocity.setToggleState(true, juce::NotificationType::dontSendNotification);
            lowVelocity.setButtonText(juce::String(currentVelocity));
        }
    } else
    {
        highVelocity.setToggleState(false, juce::NotificationType::dontSendNotification);
        lowVelocity.setToggleState(false, juce::NotificationType::dontSendNotification);
    }
}

void AudioPluginAudioProcessorEditor::handleNoteOn(juce::MidiKeyboardState *source, int midiChannel,
                                                   int midiNoteNumber, float velocity)
{
    juce::ignoreUnused(source, midiChannel, midiNoteNumber, velocity);
    DBG("Note on===");
    noteActive = true;
    currentVelocity = velocity;
}

void AudioPluginAudioProcessorEditor::handleNoteOff(juce::MidiKeyboardState *source, int midiChannel,
                                                    int midiNoteNumber, float velocity)
{
    juce::ignoreUnused(source, midiChannel, midiNoteNumber, velocity);
    DBG("Note off===");
    noteActive = false;
}
