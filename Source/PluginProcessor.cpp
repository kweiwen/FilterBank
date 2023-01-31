/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
puannhiAudioProcessor::puannhiAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::mono(), true)
                     #endif
                       )
#endif
{
    addParameter(mGain0 = new juce::AudioParameterFloat("0x00", "Gain0", 0.0f, 1.0f, 0.0f));
    addParameter(mGain1 = new juce::AudioParameterFloat("0x01", "Gain1", 0.0f, 1.0f, 0.0f));
}

puannhiAudioProcessor::~puannhiAudioProcessor()
{
}

//==============================================================================
const juce::String puannhiAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool puannhiAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool puannhiAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool puannhiAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double puannhiAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int puannhiAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int puannhiAudioProcessor::getCurrentProgram()
{
    return 0;
}

void puannhiAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String puannhiAudioProcessor::getProgramName (int index)
{
    return {};
}

void puannhiAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void puannhiAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    double L_b_coefficients[] = {0.00000104697434f, 0.00000418789734f, 0.00000628184601f, 0.00000418789734f, 0.00000104697434f};
    double H_b_coefficients[] = {0.91159345f, -3.6463738f, 5.4695607f, -3.6463738f, 0.91159345f};
    
    double a_coefficients[] = {1.0f, -3.81500325f, 5.46175145f, -3.47773597f, 0.83100453f};
    
    iir0L.reset(new IIRFilter(L_b_coefficients, a_coefficients));
    iir0H.reset(new IIRFilter(H_b_coefficients, a_coefficients));
}

void puannhiAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool puannhiAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void puannhiAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    auto* input = buffer.getReadPointer(0);
    auto* output = buffer.getWritePointer(0);

    for(int i = 0; i < getBlockSize(); i++)
    {
        auto gain0 = mGain0->get();
        auto gain1 = mGain1->get();
        output[i] = iir0L->process_sample(input[i]) * gain0 + iir0H->process_sample(input[i]) * gain1;
    }
}

//==============================================================================
bool puannhiAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* puannhiAudioProcessor::createEditor()
{
    return new puannhiAudioProcessorEditor (*this);
}

//==============================================================================
void puannhiAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void puannhiAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new puannhiAudioProcessor();
}
