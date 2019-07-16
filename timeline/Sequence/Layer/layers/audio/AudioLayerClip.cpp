/*
  ==============================================================================

	AudioLayerClip.cpp
	Created: 8 Feb 2017 12:20:02pm
	Author:  Ben

  ==============================================================================
*/

AudioLayerClip::AudioLayerClip(float _time) :
	BaseItem("Clip"),
	Thread("AudioClipReader"),
    channelRemapAudioSource(&transportSource, false),
    clipDuration(0),
	sampleRate(0),
	clipSamplePos(0),
	isCurrent(false),
	isLoading(false),
	audioClipAsyncNotifier(10)
{
	filePath = new FileParameter("File Path", "File Path", "");
	addParameter(filePath);

	time = addFloatParameter("Start Time", "Time of the start of the clip", 0, 0, 3600);
	time->setValue(_time);
	time->defaultUI = FloatParameter::TIME;

	clipLength = addFloatParameter("Length", "Length of the clip (in seconds)", 10, .1f, 3600);
	clipLength->defaultUI = FloatParameter::TIME;
	clipLength->setControllableFeedbackOnly(true);

	volume = addFloatParameter("Volume", "Volume multiplier", 1, 0, 50);
	scratch = addBoolParameter("Scratch", "Scratch when seeking", false);

	isLocked = addBoolParameter("Locked", "When locked, you can't change time or flag values", false);


	formatManager.registerBasicFormats();

}

AudioLayerClip::~AudioLayerClip()
{
	signalThreadShouldExit();
	waitForThreadToExit(3000);
	masterReference.clear();
	transportSource.releaseResources();
}

void AudioLayerClip::setIsCurrent(bool value)
{
	if (isCurrent == value) return;
	isCurrent = value;

	if (isCurrent)
	{
		clipSamplePos = 0;
	} else
	{
		transportSource.stop();
		clipSamplePos = -1;
	}

	clipListeners.call(&ClipListener::clipIsCurrentChanged, this);
	audioClipAsyncNotifier.addMessage(new ClipEvent(ClipEvent::CLIP_IS_CURRENT_CHANGED, this));
}

bool AudioLayerClip::isInRange(float _time)
{
	return (_time >= time->floatValue() && _time <= time->floatValue() + clipLength->floatValue());
}

void AudioLayerClip::updateAudioSourceFile()
{
	if (filePath->stringValue().isEmpty()) return;

#if JUCE_WINDOWS
	if (filePath->stringValue().startsWithChar('/')) return;
#endif

	isLoading = true;
	startThread();
}

void AudioLayerClip::onContainerParameterChanged(Parameter * p)
{
	if (p == filePath)
	{
		updateAudioSourceFile();
	}
}

void AudioLayerClip::run()
{
	if (filePath == nullptr) return;

	audioClipAsyncNotifier.addMessage(new ClipEvent(ClipEvent::SOURCE_LOAD_START, this));

	transportSource.setSource(nullptr);
	readerSource.reset(nullptr);

	AudioFormatReader * reader = formatManager.createReaderFor(filePath->getAbsolutePath());

	if (reader != nullptr)
	{
		std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
		transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
		readerSource.reset(newSource.release());
		
		sampleRate = reader->sampleRate;
		clipDuration = reader->lengthInSamples / sampleRate;

		clipLength->setValue(clipDuration);
		//buffer.setSize((int)reader->numChannels, (int)reader->lengthInSamples);
		//reader->read(&buffer, 0, (int)reader->lengthInSamples, 0, true, true);

	}

	isLoading = false;
	audioClipAsyncNotifier.addMessage(new ClipEvent(ClipEvent::SOURCE_LOAD_END, this));
}
