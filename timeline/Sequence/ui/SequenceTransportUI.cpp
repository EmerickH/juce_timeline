/*
  ==============================================================================

    SequenceTransportUI.cpp
    Created: 20 Nov 2016 2:24:35pm
    Author:  Ben Kuper

  ==============================================================================
*/


SequenceTransportUI::SequenceTransportUI(Sequence * _sequence) :
	sequence(_sequence),
	timeLabel(_sequence->currentTime)
{
	timeLabel.maxFontHeight = 16;
	addAndMakeVisible(&timeLabel);
	sequence->addSequenceListener(this);
	 
	
	togglePlayUI = sequence->togglePlayTrigger->createImageUI(ImageCache::getFromMemory(TimelineBinaryData::play_png, TimelineBinaryData::play_pngSize));
	stopUI = sequence->stopTrigger->createImageUI(ImageCache::getFromMemory(TimelineBinaryData::stop_png, TimelineBinaryData::stop_pngSize));
	nextCueUI = sequence->nextCue->createImageUI(ImageCache::getFromMemory(TimelineBinaryData::nextcue_png, TimelineBinaryData::nextcue_pngSize));
	prevCueUI = sequence->prevCue->createImageUI(ImageCache::getFromMemory(TimelineBinaryData::prevcue_png, TimelineBinaryData::prevcue_pngSize));
	
	addAndMakeVisible(togglePlayUI);
	addAndMakeVisible(stopUI);
	addAndMakeVisible(nextCueUI);
	addAndMakeVisible(prevCueUI);
}

SequenceTransportUI::~SequenceTransportUI()
{
	togglePlayUI = nullptr;
	stopUI = nullptr;
	nextCueUI = nullptr;
	prevCueUI = nullptr;

	sequence->removeSequenceListener(this);
}

void SequenceTransportUI::paint(Graphics &g)
{
	/*
	if (sequence->masterAudioModule != nullptr)
	{
		Rectangle<int> r = getLocalBounds().reduced(2).removeFromTop(timeLabel.getHeight()).withLeft(timeLabel.getRight() + 5);
		g.setColour(sequence->timeIsDrivenByAudio()?AUDIO_COLOR.brighter(.7f):HIGHLIGHT_COLOR);
		g.drawFittedText(sequence->masterAudioModule->niceName, r.reduced(2), Justification::centred, 1);
	}
	*/
}

void SequenceTransportUI::resized()
{
	Rectangle<int> r = getLocalBounds().reduced(2);
	timeLabel.setBounds(r.removeFromTop(20).removeFromLeft(110));
	r.removeFromTop(2);
	
	
	Rectangle<int> pr = r.removeFromTop(24);
	togglePlayUI->setBounds(pr.removeFromLeft(pr.getHeight()).reduced(4));
	stopUI->setBounds(pr.removeFromLeft(pr.getHeight()).reduced(4));
	prevCueUI->setBounds(pr.removeFromLeft(pr.getHeight()).reduced(4));
	nextCueUI->setBounds(pr.removeFromLeft(pr.getHeight()).reduced(4));
	

}

void SequenceTransportUI::sequenceMasterAudioModuleChanged(Sequence *)
{
	repaint();
}
