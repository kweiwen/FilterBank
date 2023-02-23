/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "IIRFilter.h"
#include "CircularBuffer.h"

//==============================================================================
/**
*/
class puannhiAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    puannhiAudioProcessor();
    ~puannhiAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
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
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

private:
    juce::AudioParameterFloat* mGain0;
    juce::AudioParameterFloat* mGain1;
    juce::AudioParameterFloat* mGain2;
    juce::AudioParameterFloat* mGain3;
    juce::AudioParameterFloat* mGain4;
    juce::AudioParameterFloat* mGain5;

    std::unique_ptr<IIRFilter> allpass0;
    std::unique_ptr<IIRFilter> allpass1;
    std::unique_ptr<IIRFilter> allpass2;
    std::unique_ptr<IIRFilter> allpass3;
    std::unique_ptr<IIRFilter> allpass4;
    std::unique_ptr<IIRFilter> allpass5;

    const int cbLen0 = 7 * 4;
    const int cbLen1 = 15 * 2;
    const int cbLen2 = 15;
    const int cbLen3 = 7 * 2;
    const int cbLen4 = 15;
    const int cbLen5 = 7;

    CircularBuffer<double> cb0;
    CircularBuffer<double> cb1;
    CircularBuffer<double> cb2;
    CircularBuffer<double> cb3;
    CircularBuffer<double> cb4;
    CircularBuffer<double> cb5;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(puannhiAudioProcessor)
};
