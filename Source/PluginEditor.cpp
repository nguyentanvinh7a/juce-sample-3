/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TestjuceAudioProcessorEditor::TestjuceAudioProcessorEditor (TestjuceAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    mLoadButton.onClick = [&]() {audioProcessor.loadFile(); };
    addAndMakeVisible(mLoadButton);
    
    setSize (600, 200);
}

TestjuceAudioProcessorEditor::~TestjuceAudioProcessorEditor()
{
}

//==============================================================================
void TestjuceAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(Colours::black);
    g.setColour(Colours::white);

    if (mShouldBePainting) {
        Path p;
        mAudioPoints.clear();

        auto waveform = audioProcessor.getWaveForm();
        auto ratio = waveform.getNumSamples() / getWidth();
        auto buffer = waveform.getReadPointer(0);

        //scale audio file to window on x axis
        for (int sample = 0; sample < waveform.getNumSamples(); sample += ratio) {
            mAudioPoints.push_back(buffer[sample]);
        }

        p.startNewSubPath(0, getHeight() / 2);

        //scale on y axis
        for (int sample = 0; sample < mAudioPoints.size(); ++sample) {
            auto point = jmap<float>(mAudioPoints[sample], -1.0f, 1.0f, 200, 0);
            p.lineTo(sample, point);
        }

        g.strokePath(p, PathStrokeType(2));

        mShouldBePainting = false;
    }
    //g.setColour(Colours::white);
    //g.setFont(15.f);
    //if (audioProcessor.getNumSamplerSounds() > 0) {
    //    g.fillAll(Colours::red);
    //    g.setColour(Colours::white);
    //    g.setFont(15.f);
    //    g.drawText("Sound loaded", getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 20, Justification::centred);
    //}
    //else
    //    g.drawText("Loading a sound", getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 20, Justification::centred);

}

void TestjuceAudioProcessorEditor::resized()
{
    //mLoadButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
}

bool TestjuceAudioProcessorEditor::isInterestedInFileDrag(const StringArray& files) {
    for (auto file : files) {
        if (file.contains(".wav") || file.contains(".mp3") || file.contains(".aif"))
            return true;
    }
    return false;
}
void TestjuceAudioProcessorEditor::filesDropped(const StringArray& files, int x, int y) {
    for (auto file : files) {
        if (isInterestedInFileDrag(file)) {
            mShouldBePainting = true;
            audioProcessor.loadFile(file);
        }
    }
    repaint();
}
