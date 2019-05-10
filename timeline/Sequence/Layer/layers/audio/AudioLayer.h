/*
  ==============================================================================

    AudioLayer.h
    Created: 20 Nov 2016 3:08:41pm
    Author:  Ben Kuper

  ==============================================================================
*/

#pragma once

class AudioLayerProcessor;

class AudioLayer :
	public SequenceLayer
{
public:
	AudioLayer(Sequence * sequence, var params);
	~AudioLayer();
	
	AudioLayerClipManager clipManager;
	
	AudioProcessorGraph * currentGraph;

	WeakReference<AudioLayerClip> currentClip;
	AudioLayerProcessor * currentProcessor;

	Array<BoolParameter *> outChannels;
	Array<int> selectedOutChannels;

    FloatParameter * volume;
	FloatParameter * enveloppe;

	int numActiveOutputs;

	AudioProcessorGraph::NodeID graphID;
	static int graphIDIncrement;
	int audioOutputGraphID;

	void setAudioProcessorGraph(AudioProcessorGraph * graph, int audioOutputGraphID = 2);
	void updateCurrentClip();

	void updateSelectedOutChannels();

	void onContainerParameterChangedInternal(Parameter * p) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	virtual SequenceLayerPanel * getPanel() override;
	virtual SequenceLayerTimeline * getTimelineUI() override;
	
	void sequenceCurrentTimeChanged(Sequence *, float prevTime, bool evaluatedSkippedData) override;
	void sequencePlayStateChanged(Sequence *) override;

	static AudioLayer * create(Sequence * sequence, var params) { return new AudioLayer(sequence, params); }
	virtual String getTypeString() const override { return "Audio"; }

	class AudioLayerListener
	{
	public:
		virtual ~AudioLayerListener() {}
		virtual void targetAudioModuleChanged(AudioLayer *) {}
	};

	ListenerList<AudioLayerListener> audioLayerListeners;
	void addAudioLayerListener(AudioLayerListener* newListener) { audioLayerListeners.add(newListener); }
	void removeAudioLayerListener(AudioLayerListener* listener) { audioLayerListeners.remove(listener); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioLayer)
	
};


class AudioLayerProcessor :
	public AudioProcessor
{
public:
	AudioLayerProcessor(AudioLayer * layer);
	~AudioLayerProcessor();
	
	AudioLayer * layer;
	
	const int minEnveloppeSamples = 1024;
	int rmsCount;
	float tempRMS;
	float currentEnveloppe;

	void clear();

	// H�rit� via AudioProcessor
	virtual const String getName() const override;
	virtual void prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock) override;
	virtual void releaseResources() override;
	virtual void processBlock(AudioBuffer<float>& buffer, MidiBuffer & midiMessages) override;
	virtual double getTailLengthSeconds() const override;
	virtual bool acceptsMidi() const override;
	virtual bool producesMidi() const override;
	virtual AudioProcessorEditor * createEditor() override;
	virtual bool hasEditor() const override;
	virtual int getNumPrograms() override;
	virtual int getCurrentProgram() override;
	virtual void setCurrentProgram(int index) override;
	virtual const String getProgramName(int index) override;
	virtual void changeProgramName(int index, const String & newName) override;
	virtual void getStateInformation(juce::MemoryBlock & destData) override;
	virtual void setStateInformation(const void * data, int sizeInBytes) override;
};