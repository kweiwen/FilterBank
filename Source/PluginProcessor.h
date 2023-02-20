/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "IIRFilter.h"

//==============================================================================
/**
*/
class puannhiAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    puannhiAudioProcessor();
    ~puannhiAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    juce::AudioParameterFloat* mGain0;
    juce::AudioParameterFloat* mGain1;
    juce::AudioParameterFloat* mGain2;
    juce::AudioParameterFloat* mGain3;
    juce::AudioParameterFloat* mGain4;
    
    std::unique_ptr<IIRFilter> iir0L;
    std::unique_ptr<IIRFilter> iir0H;
    std::unique_ptr<IIRFilter> iir1L;
    std::unique_ptr<IIRFilter> iir1H;
    std::unique_ptr<IIRFilter> iir2L;
    std::unique_ptr<IIRFilter> iir2H;
    std::unique_ptr<IIRFilter> iir3L;
    std::unique_ptr<IIRFilter> iir3H;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (puannhiAudioProcessor)
};
