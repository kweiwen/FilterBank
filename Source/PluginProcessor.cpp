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
    double b_0_L[] = { -0.0007770007770007757, 0.0, 0.0, 0.0, -3.469446951953614e-18, 0.0, 0.0, 0.0, 0.009324009324009341, 0.0, 0.0, 0.0, -6.938893903907228e-17, 0.0, 0.0, 0.0, -0.060606060606060524, 0.0, 0.0, 0.0, -2.7755575615628914e-17, 0.0, 0.0, 0.0, 0.4444444444444445, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.4444444444444445, 0.0, 0.0, 0.0, -2.7755575615628914e-17, 0.0, 0.0, 0.0, -0.060606060606060524, 0.0, 0.0, 0.0, -6.938893903907228e-17, 0.0, 0.0, 0.0, 0.009324009324009341, 0.0, 0.0, 0.0, -3.469446951953614e-18, 0.0, 0.0, 0.0, -0.0007770007770007757 };
    double a_0[] = { 2.0, 8.881784197001252e-15, -3.319566843629218e-14, 5.3179682879545e-14, 1.7863488466218769e-13, -1.3321872626483008e-12, 4.180156221167408e-12, -8.111483706940703e-12, 0.8888888888985511, -4.006217625417014e-12, -1.075994848775963e-11, 3.113326263459726e-11, -5.0552784180979415e-11, 6.35941713193889e-11, -6.788165862747775e-11, 6.375178163153805e-11, -0.12121212126570492, 4.095834137294662e-11, -2.9170822875290714e-11, 1.9846586180038983e-11, -1.2940529290489389e-11, 7.830838614675384e-12, -4.168364221499021e-12, 1.8485111445004643e-12, 0.0186480186473665, 1.7794288931896095e-13, -3.930709924215847e-14, 9.295515746021721e-15, -2.899590290095233e-15, 9.146076180335876e-16, -1.5243882545146192e-16, 1.3227266504323154e-17, -0.0015540015540015683 };
    iir0L.reset(new IIRFilter(b_0_L, a_0));

    double b_3_L[] = { -0.0007770007770007757, 0.0, -3.469446951953614e-18, 0.0, 0.009324009324009341, 0.0, -6.938893903907228e-17, 0.0, -0.060606060606060524, 0.0, -2.7755575615628914e-17, 0.0, 0.4444444444444445, 0.0, 1.0, 0.0, 1.0, 0.0, 0.4444444444444445, 0.0, -2.7755575615628914e-17, 0.0, -0.060606060606060524, 0.0, -6.938893903907228e-17, 0.0, 0.009324009324009341, 0.0, -3.469446951953614e-18, 0.0, -0.0007770007770007757 };
    double a_3[] = { 2.0, 3.3306690738754696e-15, -3.9968028886505635e-15, 3.6637359812630166e-15, 0.8888888888888923, 1.5543122344752192e-15, 2.7755575615628914e-16, -8.604228440844963e-16, -0.12121212121212016, -5.724587470723463e-16, -1.5439038936193583e-16, -4.683753385137379e-17, 0.01864801864801841, 3.642919299551295e-17, -6.938893903907228e-18, -2.3418766925686896e-17, -0.0015540015540015455 };
    iir3L.reset(new IIRFilter(b_3_L, a_3));

    double b_5_L[] = { -0.0007770007770007757, -3.469446951953614e-18, 0.009324009324009341, -6.938893903907228e-17, -0.060606060606060524, -2.7755575615628914e-17, 0.4444444444444445, 1.0, 1.0, 0.4444444444444445, -2.7755575615628914e-17, -0.060606060606060524, -6.938893903907228e-17, 0.009324009324009341, -3.469446951953614e-18, -0.0007770007770007757 };
    double b_5_H[] = { -0.0007770007770007757, -3.469446951953614e-18, 0.009324009324009341, -6.938893903907228e-17, -0.060606060606060524, -2.7755575615628914e-17, 0.4444444444444445, -1.0, 1.0, -0.4444444444444445, 2.7755575615628914e-17, 0.060606060606060524, 6.938893903907228e-17, -0.009324009324009341, 3.469446951953614e-18, 0.0007770007770007757 };
    double a_5[] = { 2.0, -1.887379141862766e-15, 0.8888888888888911, -7.771561172376096e-16, -0.12121212121212149, -9.454242944073599e-17, 0.018648018648018717, -2.6020852139652106e-17, -0.0015540015540015566 };
    iir5L.reset(new IIRFilter(b_5_L, a_5));
    iir5H.reset(new IIRFilter(b_5_H, a_5));

    double b_1_L[] = { -1.4274296987130925e-06, 0.0, 5.421010862427522e-19, 0.0, 2.7230966560062506e-05, 0.0, -3.469446951953614e-18, 0.0, -0.0002512675550769457, 0.0, 6.591949208711867e-17, 0.0, 0.0015076053304614312, 0.0, 3.8510861166685117e-16, 0.0, -0.006730380939561602, 0.0, 8.673617379884035e-17, 0.0, 0.024767801857584815, 0.0, 1.3877787807814457e-16, 0.0, -0.0866873065015481, 0.0, 0.0, 0.0, 0.47058823529411764, 0.0, 1.0, 0.0, 1.0, 0.0, 0.47058823529411764, 0.0, 0.0, 0.0, -0.0866873065015481, 0.0, 1.3877787807814457e-16, 0.0, 0.024767801857584815, 0.0, 8.673617379884035e-17, 0.0, -0.006730380939561602, 0.0, 3.8510861166685117e-16, 0.0, 0.0015076053304614312, 0.0, 6.591949208711867e-17, 0.0, -0.0002512675550769457, 0.0, -3.469446951953614e-18, 0.0, 2.7230966560062506e-05, 0.0, 5.421010862427522e-19, 0.0, -1.4274296987130925e-06 };
    double a_1[] = { 2.0, 1.1102230246251565e-15, 2.0761170560490427e-14, -6.661338147750939e-16, 0.9411764705882029, 7.893685705084863e-14, -1.0291767438275201e-13, 1.4935275238769918e-13, -0.17337461300329196, 2.0693516344927332e-13, -1.823662748590138e-13, 1.4334367026691552e-13, 0.049535603715030095, 1.9000079287678773e-13, -2.4272250875867485e-13, 2.489050632270562e-13, -0.013460761879316697, 1.1344137434976531e-13, -4.6736919889767137e-14, 1.190887666258078e-14, 0.003015210660924892, -3.753291080710319e-15, 3.4445103019864476e-15, -1.6115581091824538e-15, -0.0005025351101530915, -2.1597307275911248e-16, 5.926520125348889e-17, -1.1756817307889689e-17, 5.4461933120072156e-05, 1.7787691892340307e-19, 1.6737371037744975e-18, -1.9481757786848908e-20, -2.8548593974262185e-06 };
    iir1L.reset(new IIRFilter(b_1_L, a_1));

    double b_2_L[] = { -1.4274296987130925e-06, 5.421010862427522e-19, 2.7230966560062506e-05, -3.469446951953614e-18, -0.0002512675550769457, 6.591949208711867e-17, 0.0015076053304614312, 3.8510861166685117e-16, -0.006730380939561602, 8.673617379884035e-17, 0.024767801857584815, 1.3877787807814457e-16, -0.0866873065015481, 0.0, 0.47058823529411764, 1.0, 1.0, 0.47058823529411764, 0.0, -0.0866873065015481, 1.3877787807814457e-16, 0.024767801857584815, 8.673617379884035e-17, -0.006730380939561602, 3.8510861166685117e-16, 0.0015076053304614312, 6.591949208711867e-17, -0.0002512675550769457, -3.469446951953614e-18, 2.7230966560062506e-05, 5.421010862427522e-19, -1.4274296987130925e-06 };
    double b_2_H[] = { -1.4274296987130925e-06, 5.421010862427522e-19, 2.7230966560062506e-05, -3.469446951953614e-18, -0.0002512675550769457, 6.591949208711867e-17, 0.0015076053304614312, 3.8510861166685117e-16, -0.006730380939561602, 8.673617379884035e-17, 0.024767801857584815, 1.3877787807814457e-16, -0.0866873065015481, 0.0, 0.47058823529411764, -1.0, 1.0, -0.47058823529411764, 0.0, 0.0866873065015481, -1.3877787807814457e-16, -0.024767801857584815, -8.673617379884035e-17, 0.006730380939561602, -3.8510861166685117e-16, -0.0015076053304614312, -6.591949208711867e-17, 0.0002512675550769457, 3.469446951953614e-18, -2.7230966560062506e-05, -5.421010862427522e-19, 1.4274296987130925e-06 };
    double a_2[] = { 2.0, -4.551914400963142e-15, 0.941176470588233, -5.662137425588298e-15, -0.17337461300309023, -8.569533971325427e-16, 0.04953560371516899, 9.992007221626409e-16, -0.013460761879123484, 9.900934239137626e-16, 0.0030152106609227253, 1.4040418133687282e-16, -0.0005025351101539204, -6.572975670693371e-18, 5.446193312012463e-05, 1.085896238380013e-18, -2.854859397426167e-06 };
    iir2L.reset(new IIRFilter(b_2_L, a_2));
    iir2H.reset(new IIRFilter(b_2_H, a_2));

    double b_4_L[] = { -1.4274296987130925e-06, 5.421010862427522e-19, 2.7230966560062506e-05, -3.469446951953614e-18, -0.0002512675550769457, 6.591949208711867e-17, 0.0015076053304614312, 3.8510861166685117e-16, -0.006730380939561602, 8.673617379884035e-17, 0.024767801857584815, 1.3877787807814457e-16, -0.0866873065015481, 0.0, 0.47058823529411764, 1.0, 1.0, 0.47058823529411764, 0.0, -0.0866873065015481, 1.3877787807814457e-16, 0.024767801857584815, 8.673617379884035e-17, -0.006730380939561602, 3.8510861166685117e-16, 0.0015076053304614312, 6.591949208711867e-17, -0.0002512675550769457, -3.469446951953614e-18, 2.7230966560062506e-05, 5.421010862427522e-19, -1.4274296987130925e-06 };
    double b_4_H[] = { -1.4274296987130925e-06, 5.421010862427522e-19, 2.7230966560062506e-05, -3.469446951953614e-18, -0.0002512675550769457, 6.591949208711867e-17, 0.0015076053304614312, 3.8510861166685117e-16, -0.006730380939561602, 8.673617379884035e-17, 0.024767801857584815, 1.3877787807814457e-16, -0.0866873065015481, 0.0, 0.47058823529411764, -1.0, 1.0, -0.47058823529411764, 0.0, 0.0866873065015481, -1.3877787807814457e-16, -0.024767801857584815, -8.673617379884035e-17, 0.006730380939561602, -3.8510861166685117e-16, -0.0015076053304614312, -6.591949208711867e-17, 0.0002512675550769457, 3.469446951953614e-18, -2.7230966560062506e-05, -5.421010862427522e-19, 1.4274296987130925e-06 };
    double a_4[] = { 2.0, -4.551914400963142e-15, 0.941176470588233, -5.662137425588298e-15, -0.17337461300309023, -8.569533971325427e-16, 0.04953560371516899, 9.992007221626409e-16, -0.013460761879123484, 9.900934239137626e-16, 0.0030152106609227253, 1.4040418133687282e-16, -0.0005025351101539204, -6.572975670693371e-18, 5.446193312012463e-05, 1.085896238380013e-18, -2.854859397426167e-06 };
    iir4L.reset(new IIRFilter(b_4_L, a_4));
    iir4H.reset(new IIRFilter(b_4_H, a_4));

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
        auto gain2 = mGain2->get();
        auto gain3 = mGain3->get();
        auto gain4 = mGain4->get();
        auto gain5 = mGain5->get();


        double h_0_l = iir0L->process_sample(input[i]);
        double h_1_l = iir1L->process_sample(h_0_l);
        double channel_0 = iir2L->process_sample(h_1_l);
        double channel_5 = iir2H->process_sample(h_1_l);

        double h_3_l = iir3L->process_sample(input[i]);
        double h_4_l = iir4L->process_sample(h_3_l);
        double h_4_h = iir4H->process_sample(h_3_l);

        double channel_1 = h_4_l - channel_0;
        double channel_4 = h_4_h - channel_5;

        double h_5_l = iir5L->process_sample(input[i]);
        double h_5_h = iir5H->process_sample(input[i]);

        double channel_2 = h_5_l - channel_1;
        double channel_3 = h_5_h - channel_4;

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
