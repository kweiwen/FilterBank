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
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::mono(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::mono(), true)
#endif
    )
#endif
{
    addParameter(mGain0 = new juce::AudioParameterFloat("0x00", "Gain0", 0.0f, 1.0f, 0.0f));
    addParameter(mGain1 = new juce::AudioParameterFloat("0x01", "Gain1", 0.0f, 1.0f, 0.0f));
    addParameter(mGain2 = new juce::AudioParameterFloat("0x02", "Gain2", 0.0f, 1.0f, 0.0f));
    addParameter(mGain3 = new juce::AudioParameterFloat("0x03", "Gain3", 0.0f, 1.0f, 0.0f));
    addParameter(mGain4 = new juce::AudioParameterFloat("0x04", "Gain4", 0.0f, 1.0f, 0.0f));
    addParameter(mGain5 = new juce::AudioParameterFloat("0x05", "Gain5", 0.0f, 1.0f, 0.0f));
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

void puannhiAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String puannhiAudioProcessor::getProgramName(int index)
{
    return {};
}

void puannhiAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void puannhiAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    double allpass0_b[] = { -0.0007770007770007757, 0, 0, 0, -3.469446951953614e-18, 0, 0, 0, 0.009324009324009341, 0, 0, 0, -6.938893903907228e-17, 0, 0, 0, -0.060606060606060524, 0, 0, 0, -2.7755575615628914e-17, 0, 0, 0, 0.4444444444444445, 0, 0, 0, 0.0, 0, 0, 0, 1.0 };
    double allpass0_a[] = { 1.0, 0, 0, 0, 0.0, 0, 0, 0, 0.4444444444444445, 0, 0, 0, -2.7755575615628914e-17, 0, 0, 0, -0.060606060606060524, 0, 0, 0, -6.938893903907228e-17, 0, 0, 0, 0.009324009324009341, 0, 0, 0, -3.469446951953614e-18, 0, 0, 0, -0.0007770007770007757 };
    allpass0.reset(new IIRFilter(allpass0_b, allpass0_a));

    double allpass1_b[] = { -1.4274296987130925e-06, 0, 5.421010862427522e-19, 0, 2.7230966560062506e-05, 0, -3.469446951953614e-18, 0, -0.0002512675550769457, 0, 6.591949208711867e-17, 0, 0.0015076053304614312, 0, 3.8510861166685117e-16, 0, -0.006730380939561602, 0, 8.673617379884035e-17, 0, 0.024767801857584815, 0, 1.3877787807814457e-16, 0, -0.0866873065015481, 0, 0.0, 0, 0.47058823529411764, 0, 0.0, 0, 1.0 };
    double allpass1_a[] = { 1.0, 0, 0.0, 0, 0.47058823529411764, 0, 0.0, 0, -0.0866873065015481, 0, 1.3877787807814457e-16, 0, 0.024767801857584815, 0, 8.673617379884035e-17, 0, -0.006730380939561602, 0, 3.8510861166685117e-16, 0, 0.0015076053304614312, 0, 6.591949208711867e-17, 0, -0.0002512675550769457, 0, -3.469446951953614e-18, 0, 2.7230966560062506e-05, 0, 5.421010862427522e-19, 0, -1.4274296987130925e-06 };
    allpass1.reset(new IIRFilter(allpass1_b, allpass1_a));

    double allpass2_b[] = { -1.4274296987130925e-06, 5.421010862427522e-19, 2.7230966560062506e-05, -3.469446951953614e-18, -0.0002512675550769457, 6.591949208711867e-17, 0.0015076053304614312, 3.8510861166685117e-16, -0.006730380939561602, 8.673617379884035e-17, 0.024767801857584815, 1.3877787807814457e-16, -0.0866873065015481, 0.0, 0.47058823529411764, 0.0, 1.0 };
    double allpass2_a[] = { 1.0, 0.0, 0.47058823529411764, 0.0, -0.0866873065015481, 1.3877787807814457e-16, 0.024767801857584815, 8.673617379884035e-17, -0.006730380939561602, 3.8510861166685117e-16, 0.0015076053304614312, 6.591949208711867e-17, -0.0002512675550769457, -3.469446951953614e-18, 2.7230966560062506e-05, 5.421010862427522e-19, -1.4274296987130925e-06 };
    allpass2.reset(new IIRFilter(allpass2_b, allpass2_a));

    double allpass3_b[] = { -0.0007770007770007757, 0, -3.469446951953614e-18, 0, 0.009324009324009341, 0, -6.938893903907228e-17, 0, -0.060606060606060524, 0, -2.7755575615628914e-17, 0, 0.4444444444444445, 0, 0.0, 0, 1.0 };
    double allpass3_a[] = { 1.0, 0, 0.0, 0, 0.4444444444444445, 0, -2.7755575615628914e-17, 0, -0.060606060606060524, 0, -6.938893903907228e-17, 0, 0.009324009324009341, 0, -3.469446951953614e-18, 0, -0.0007770007770007757 };
    allpass3.reset(new IIRFilter(allpass3_b, allpass3_a));

    double allpass4_b[] = { -1.4274296987130925e-06, 5.421010862427522e-19, 2.7230966560062506e-05, -3.469446951953614e-18, -0.0002512675550769457, 6.591949208711867e-17, 0.0015076053304614312, 3.8510861166685117e-16, -0.006730380939561602, 8.673617379884035e-17, 0.024767801857584815, 1.3877787807814457e-16, -0.0866873065015481, 0.0, 0.47058823529411764, 0.0, 1.0 };
    double allpass4_a[] = { 1.0, 0.0, 0.47058823529411764, 0.0, -0.0866873065015481, 1.3877787807814457e-16, 0.024767801857584815, 8.673617379884035e-17, -0.006730380939561602, 3.8510861166685117e-16, 0.0015076053304614312, 6.591949208711867e-17, -0.0002512675550769457, -3.469446951953614e-18, 2.7230966560062506e-05, 5.421010862427522e-19, -1.4274296987130925e-06 };
    allpass4.reset(new IIRFilter(allpass4_b, allpass4_a));

    double allpass5_b[] = { -0.0007770007770007757, -3.469446951953614e-18, 0.009324009324009341, -6.938893903907228e-17, -0.060606060606060524, -2.7755575615628914e-17, 0.4444444444444445, 0.0, 1.0 };
    double allpass5_a[] = { 1.0, 0.0, 0.4444444444444445, -2.7755575615628914e-17, -0.060606060606060524, -6.938893903907228e-17, 0.009324009324009341, -3.469446951953614e-18, -0.0007770007770007757 };
    allpass5.reset(new IIRFilter(allpass5_b, allpass5_a));

    // iir0H.reset(new IIRFilter(H_b_coefficients, a_coefficients));

    cb0.createCircularBuffer(cbLen0);
    cb0.flushBuffer();

    cb1.createCircularBuffer(cbLen1);
    cb1.flushBuffer();

    cb2.createCircularBuffer(cbLen2);
    cb2.flushBuffer();

    cb3.createCircularBuffer(cbLen3);
    cb3.flushBuffer();

    cb4.createCircularBuffer(cbLen4);
    cb4.flushBuffer();

    cb5.createCircularBuffer(cbLen5);
    cb5.flushBuffer();
}

void puannhiAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool puannhiAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
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

void puannhiAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    auto* input = buffer.getReadPointer(0);
    auto* output = buffer.getWritePointer(0);

    for (int i = 0; i < getBlockSize(); i++)
    {
        auto gain0 = mGain0->get();
        auto gain1 = mGain1->get();
        auto gain2 = mGain2->get();
        auto gain3 = mGain3->get();
        auto gain4 = mGain4->get();
        auto gain5 = mGain5->get();

        double current_sample = input[i];
        double filter0 = allpass0->process_sample(current_sample);
        double delay0 = cb0.readBuffer(cbLen0);
        cb0.writeBuffer(current_sample);
        double h_0_L = 0.5 * (filter0 + delay0);

        double filter1 = allpass1->process_sample(h_0_L);
        double delay1 = cb1.readBuffer(cbLen1);
        cb1.writeBuffer(h_0_L);
        double h_1_L = 0.5 * (filter1 + delay1);

        double filter2 = allpass2->process_sample(h_1_L);
        double delay2 = cb2.readBuffer(cbLen2);
        cb2.writeBuffer(h_1_L);
        double channel_0 = 0.5 * (filter2 + delay2);
        double channel_5 = 0.5 * (filter2 - delay2);

        double filter3 = allpass3->process_sample(current_sample);
        double delay3 = cb3.readBuffer(cbLen3);
        cb3.writeBuffer(current_sample);
        double h_3_L = 0.5 * (filter3 + delay3);

        double filter4 = allpass4->process_sample(h_3_L);
        double delay4 = cb4.readBuffer(cbLen4);
        cb4.writeBuffer(h_3_L);
        double channel_1 = 0.5 * (filter4 + delay4) - channel_0;
        double channel_4 = 0.5 * (filter4 - delay4) - channel_5;

        double filter5 = allpass5->process_sample(current_sample);
        double delay5 = cb5.readBuffer(cbLen5);
        cb5.writeBuffer(current_sample);
        double channel_2 = 0.5 * (filter5 + delay5) - channel_1;
        double channel_3 = 0.5 * (filter5 - delay5) - channel_4;

        output[i] = channel_0 * gain0 + channel_1 * gain1 + channel_2 * gain2 + channel_3 * gain3 + channel_4 * gain4 + channel_5 * gain5;
    }
}

//==============================================================================
bool puannhiAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* puannhiAudioProcessor::createEditor()
{
    return new puannhiAudioProcessorEditor(*this);
}

//==============================================================================
void puannhiAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void puannhiAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
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


