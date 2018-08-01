/*
  ==============================================================================

	AudioLayer.cpp
	Created: 20 Nov 2016 3:08:41pm
	Author:  Ben Kuper

  ==============================================================================
*/

int AudioLayer::graphIDIncrement = 10;

AudioLayer::AudioLayer(Sequence * _sequence, var params) :
	SequenceLayer(_sequence, "New Audio Layer"),
	currentGraph(nullptr),
	currentProcessor(nullptr),
	numActiveOutputs(0), 
    graphID(0) //was -1 but since 5.2.1, generated warning. Should do otherwise ?
{

	
    volume = addFloatParameter("Volume","Volume multiplier for the layer",1,0,10);
    
	enveloppe = addFloatParameter("Enveloppe", "Enveloppe", 0, 0, 1);
	enveloppe->isControllableFeedbackOnly = true;

	addChildControllableContainer(&clipManager);

	helpID = "AudioLayer";

}

AudioLayer::~AudioLayer()
{
	//if (ModuleManager::getInstanceWithoutCreating() != nullptr) ModuleManager::getInstance()->removeBaseManagerListener(this);
	//setAudioModule(nullptr);
	setAudioProcessorGraph(nullptr);
}

void AudioLayer::setAudioProcessorGraph(AudioProcessorGraph * graph, int outputGraphID)
{
	if (currentGraph != nullptr)
	{
		currentGraph->removeNode(graphID);
		currentProcessor->clear();
		currentProcessor = nullptr;

		for (auto &b : outChannels) removeControllable(b);
		outChannels.clear();
	}

	currentGraph = graph;

	if (currentGraph != nullptr)
	{
        
		currentProcessor = new AudioLayerProcessor(this);
		
        graphID = AudioProcessorGraph::NodeID(AudioLayer::graphIDIncrement++);
		currentGraph->addNode(currentProcessor, graphID);
		
		int numChannels = currentGraph->getMainBusNumOutputChannels();
		AudioChannelSet channelSet = currentGraph->getChannelLayoutOfBus(false, 0);
		for (int i = 0; i < numChannels; i++)
		{
			String channelName = AudioChannelSet::getChannelTypeName(channelSet.getTypeOfChannel(i));
			BoolParameter * b = addBoolParameter("Channel Out : " + channelName, "If enabled, sends audio from this layer to this channel", i < 2);
			outChannels.add(b);
		}
	}

	audioOutputGraphID = outputGraphID;

	updateSelectedOutChannels();
}


void AudioLayer::updateCurrentClip()
{
	AudioLayerClip * target = nullptr;
	
	if (sequence->currentTime->floatValue() > 0 || sequence->isPlaying->boolValue()) // only find a clip if sequence not at 0 or is playing
	{
		if (currentClip != nullptr && currentClip->isInRange(sequence->currentTime->floatValue())) return;
		target = clipManager.getClipAtTime(sequence->currentTime->floatValue());
	}

	if (target == currentClip) return;

	if (currentClip != nullptr)
	{
		currentClip->setIsCurrent(false);

	}

	currentClip = target;

	if (currentClip != nullptr)
	{
		currentClip->setIsCurrent(true);
		currentClip->clipSamplePos = (sequence->currentTime->floatValue() - currentClip->time->floatValue())*currentClip->sampleRate;
	}

}

void AudioLayer::updateSelectedOutChannels()
{
	
	selectedOutChannels.clear();

	if (currentGraph == nullptr) return;

	currentGraph->disconnectNode(graphID);

	numActiveOutputs = 0;
	for (int i = 0; i < outChannels.size(); i++) if (outChannels[i]) numActiveOutputs++;


	currentProcessor->setPlayConfigDetails(0, numActiveOutputs, currentGraph->getSampleRate(), currentGraph->getBlockSize());
	currentProcessor->prepareToPlay(currentGraph->getSampleRate(), currentGraph->getBlockSize());

	int index = 0;
	for (int i = 0; i < outChannels.size(); i++)
	{
		if (outChannels[i]->boolValue())
		{
			selectedOutChannels.add(i);
			currentGraph->addConnection({{graphID, index }, { (AudioProcessorGraph::NodeID)audioOutputGraphID, i } });
			index++;
			DBG("Send out to channel : " << outChannels[i]->niceName);
		}
	}

	

	numActiveOutputs = selectedOutChannels.size();

}


void AudioLayer::onContainerParameterChangedInternal(Parameter * p)
{
	SequenceLayer::onContainerParameterChangedInternal(p);

	if (p->type == Controllable::BOOL && outChannels.indexOf((BoolParameter *)p) > -1)
	{
		updateSelectedOutChannels();
	}
	
}

var AudioLayer::getJSONData()
{
	var data = SequenceLayer::getJSONData();
	data.getDynamicObject()->setProperty("clipManager", clipManager.getJSONData());
	//if (audioModule != nullptr) data.getDynamicObject()->setProperty("audioModule", audioModule->shortName);
	return data;
}

void AudioLayer::loadJSONDataInternal(var data)
{
	SequenceLayer::loadJSONDataInternal(data);
	clipManager.loadJSONData(data.getProperty("clipManager", var()));
	//if (data.getDynamicObject()->hasProperty("audioModule")) setAudioModule(dynamic_cast<AudioModule *>(ModuleManager::getInstance()->getItemWithName(data.getProperty("audioModule", ""))));
}

SequenceLayerPanel * AudioLayer::getPanel()
{
	return new AudioLayerPanel(this);
}

SequenceLayerTimeline * AudioLayer::getTimelineUI()
{
	return new AudioLayerTimeline(this);
}

void AudioLayer::sequenceCurrentTimeChanged(Sequence *, float, bool)
{
	if (currentProcessor != nullptr) enveloppe->setValue(currentProcessor->currentEnveloppe);
	else enveloppe->setValue(0);
	updateCurrentClip();
}

void AudioLayer::sequencePlayStateChanged(Sequence *)
{
	//
}


//Audio Processor

AudioLayerProcessor::AudioLayerProcessor(AudioLayer * _layer) :
	layer(_layer),
	rmsCount(0),
	tempRMS(0),
	currentEnveloppe(0)
{
}

AudioLayerProcessor::~AudioLayerProcessor()
{
	layer = nullptr;
}

void AudioLayerProcessor::clear()
{
	layer = nullptr;
}

const String AudioLayerProcessor::getName() const
{
	return String();
}

void AudioLayerProcessor::prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock)
{
}

void AudioLayerProcessor::releaseResources()
{

}

void AudioLayerProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer & midiMessages)
{
	buffer.clear();

	bool noProcess = false;

	WeakReference<AudioLayerClip> currentClip;

	if (layer != nullptr)
	{
		if (!layer->enabled->boolValue()) noProcess = true;;
		currentClip = layer->currentClip;
		if (currentClip.wasObjectDeleted() || currentClip.get() == nullptr) noProcess = true;
		else if (currentClip->filePath->stringValue().isEmpty()) noProcess = true;
	} else
	{
		noProcess = true;
	}

	if (noProcess)
	{
		currentEnveloppe = 0;
		rmsCount = 0;
		tempRMS = 0;
		return;
	}


	AudioSampleBuffer * clipBuffer = &layer->currentClip->buffer;
	//int numClipSamples = clipBuffer->getNumSamples();

	float position = layer->sequence->hiResAudioTime - layer->currentClip->time->floatValue();

	int curSamplePos = currentClip->clipSamplePos;
	int targetSamplePos = position * currentClip->sampleRate;

	//smoothing vynil-like when not playing
	if (!layer->sequence->isPlaying->boolValue() && currentClip->scratch->boolValue()) targetSamplePos = curSamplePos + (targetSamplePos - curSamplePos) *.1f;


	int numDiffSamples = abs(targetSamplePos - curSamplePos);
	if (numDiffSamples == 0) return;

	int numBufferSamples = buffer.getNumSamples();

	float volumeFactor = currentClip->volume->floatValue() * layer->volume->floatValue();// *layer->audioModule->outVolume->floatValue();


	int bufferChannels = buffer.getNumChannels();

	int numChannels = std::min(bufferChannels, layer->numActiveOutputs);
	int numBufferChannels = clipBuffer->getNumChannels();
	int maxChannels = std::max(numChannels, numBufferChannels);

	//DBG("Num channels " << bufferChannels << "/" << layer->numActiveOutputs);

	//int samplePosition = layer->currentClip->clipSamplePos;// *layer->currentClip->sampleRate;
	//int maxSamples = jmin<int>(buffer.getNumSamples(), clipBuffer->getNumSamples() - samplePosition)

	if (numChannels > 0)
	{
		for (int i = 0; i < maxChannels; i++)
		{
			int targetBufferChannel = i%numChannels;
			int targetChannel = i%numBufferChannels;

			int prevSampleInClip = 0;

			for (int j = 0; j < numBufferSamples; j++)
			{
				float val = 0;
				int sampleInClip = jlimit<int>(0, clipBuffer->getNumSamples() - 1, curSamplePos + (targetSamplePos - curSamplePos) * j*1.f / numBufferSamples);

				if (j > 0 && sampleInClip != prevSampleInClip)
				{
					int step = sampleInClip > prevSampleInClip ? 1 : -1;
					for (int k = prevSampleInClip; k != sampleInClip; k += step)
					{
						val += clipBuffer->getSample(targetChannel, k);
					}
					val /= abs(sampleInClip - prevSampleInClip);
				} else
				{
					val = clipBuffer->getSample(targetChannel, sampleInClip);
				}

				//DBG("Add sample in channel " << i);
				buffer.addSample(targetBufferChannel, j, val * volumeFactor);

				prevSampleInClip = sampleInClip;

			}
			//buffer.copyFrom(targetChannel, 0, *clipBuffer, i, samplePosition, maxSamples);
		}
	}
	

	currentClip->clipSamplePos = targetSamplePos;

	tempRMS += buffer.getRMSLevel(0, 0, buffer.getNumSamples());
	if (rmsCount*buffer.getNumSamples() > minEnveloppeSamples)
	{
		currentEnveloppe = tempRMS / rmsCount;
		tempRMS = 0;
		rmsCount = 0;
	} else
	{
		rmsCount++;
	}
}

double AudioLayerProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

bool AudioLayerProcessor::acceptsMidi() const
{
	return false;
}

bool AudioLayerProcessor::producesMidi() const
{
	return false;
}

AudioProcessorEditor * AudioLayerProcessor::createEditor()
{
	return nullptr;
}

bool AudioLayerProcessor::hasEditor() const
{
	return false;
}

int AudioLayerProcessor::getNumPrograms()
{
	return 0;
}

int AudioLayerProcessor::getCurrentProgram()
{
	return 0;
}

void AudioLayerProcessor::setCurrentProgram(int index)
{
}

const String AudioLayerProcessor::getProgramName(int index)
{
	return String();
}

void AudioLayerProcessor::changeProgramName(int index, const String & newName)
{
}

void AudioLayerProcessor::getStateInformation(juce::MemoryBlock & destData)
{
}

void AudioLayerProcessor::setStateInformation(const void * data, int sizeInBytes)
{
}
