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
    addParameter(mGain0 = new juce::AudioParameterFloat("0x00", "band 1", 0.0f, 1.0f, 0.0f));
    addParameter(mGain1 = new juce::AudioParameterFloat("0x01", "band 2", 0.0f, 1.0f, 0.0f));
    addParameter(mGain2 = new juce::AudioParameterFloat("0x02", "band 3", 0.0f, 1.0f, 0.0f));
    addParameter(mGain3 = new juce::AudioParameterFloat("0x03", "band 4", 0.0f, 1.0f, 0.0f));
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
    // 12dB LinkWitz-Riley Filter(125, 500, 2000)
    // F1  [array([-0.000065853566832, -0.000131707133664, -0.000065853566832]), array([1., -1.96753991575317, 0.967803330020497])]
    // F2  [array([-0.00100482000787, -0.002009640015741, -0.00100482000787]), array([1., -1.873204415984123, 0.877223696015604])]
    // F3  [array([-0.013534182630736, -0.027068365261471, -0.013534182630736]), array([1., -1.534653975957921, 0.588790706480864])]
    // F1_ [array([0.983835811443417, -1.967671622886834, 0.983835811443417]), array([1., -1.96753991575317, 0.967803330020497])]
    // F2_ [array([0.937607027999932, -1.875214055999864, 0.937607027999932]), array([1., -1.873204415984123, 0.877223696015604])]
    // F3_ [array([0.780861170609696, -1.561722341219392, 0.780861170609696]), array([1., -1.534653975957921, 0.588790706480864])]

    double L1_b_coefficients[] = { -0.000065853566832, -0.000131707133664, -0.000065853566832 };
    double L2_b_coefficients[] = { -0.00100482000787 , -0.002009640015741, -0.00100482000787 };
    double L3_b_coefficients[] = { -0.013534182630736, -0.027068365261471, -0.013534182630736 };

    double H1_b_coefficients[] = { 0.983835811443417, -1.967671622886834,  0.983835811443417 };
    double H2_b_coefficients[] = { 0.937607027999932, -1.875214055999864,  0.937607027999932 };
    double H3_b_coefficients[] = { 0.780861170609696, -1.561722341219392,  0.780861170609696 };

    double a1_coefficients[] = { 1.0f , -1.96753991575317 ,  0.967803330020497 };
    double a2_coefficients[] = { 1.0f , -1.873204415984123,  0.877223696015604 };
    double a3_coefficients[] = { 1.0f , -1.534653975957921,  0.588790706480864 };

    C1.reset(new Complementary(L1_b_coefficients, H1_b_coefficients, a1_coefficients));
    C2.reset(new Complementary(L2_b_coefficients, H2_b_coefficients, a2_coefficients));
    C3.reset(new Complementary(L3_b_coefficients, H3_b_coefficients, a3_coefficients));
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
    double output_C1[] = {0.0f, 0.0f};
    double output_C2[] = {0.0f, 0.0f};
    double output_C3[] = {0.0f, 0.0f};

    for(int i = 0; i < getBlockSize(); i++)
    {
        auto gain0 = mGain0->get();
        auto gain1 = mGain1->get();
        auto gain2 = mGain2->get();
        auto gain3 = mGain3->get();
        
        C2->process_sample_complementary(input[i], output_C2);

        auto A31 = C3->process_sample_allpass(output_C2[0]);
        auto A11 = C1->process_sample_allpass(output_C2[1]);

        C1->process_sample_complementary(A31, output_C1);
        C3->process_sample_complementary(A11, output_C3);

        output[i] = output_C1[0] * gain0 + output_C1[1] * gain1 + output_C3[0] * gain2 + output_C3[1] * gain3;
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
