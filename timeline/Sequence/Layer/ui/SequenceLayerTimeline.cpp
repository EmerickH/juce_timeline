/*
  ==============================================================================

    SequenceLayerTimeline.cpp
    Created: 28 Oct 2016 8:17:29pm
    Author:  bkupe

  ==============================================================================
*/

SequenceLayerTimeline::SequenceLayerTimeline(SequenceLayer * layer) :
	BaseItemMinimalUI<SequenceLayer>(layer),
	timeBarColor(defaultTimeBarColor)
{
	item->sequence->addAsyncContainerListener(this);
	setSize(0, item->uiHeight->intValue());

	//setInterceptsMouseClicks(true, true);
	setWantsKeyboardFocus(false);
	setMouseClickGrabsKeyboardFocus(false);
}

SequenceLayerTimeline::~SequenceLayerTimeline()
{
	item->sequence->removeAsyncContainerListener(this);
}


int SequenceLayerTimeline::getXForTime(float time)
{
	float viewStart = item->sequence->viewStartTime->floatValue();
	float viewEnd = item->sequence->viewEndTime->floatValue();
	if (viewStart == viewEnd) return 0;
	return (int)jmap<float>(time, viewStart, viewEnd, 0, (float)getWidth());
}

float SequenceLayerTimeline::getTimeForX(int tx, bool offsetStart)
{
	float viewStart = item->sequence->viewStartTime->floatValue();
	float viewEnd = item->sequence->viewEndTime->floatValue();
	float viewTime = viewEnd - viewStart;
	float mapStart = offsetStart ? viewStart : 0;
	return jmap<float>((float)tx, 0, (float)getWidth(), mapStart,mapStart+viewTime);
}



void SequenceLayerTimeline::paintOverChildren(Graphics & g)
{

	g.setColour(timeBarColor);
	g.drawVerticalLine(getXForTime(item->sequence->currentTime->floatValue()), 0, (float)getHeight());

	g.setColour(item->color->getColor());
	g.drawRoundedRectangle(getLocalBounds().reduced(1).toFloat(), 2, 2);

	BaseItemMinimalUI::paintOverChildren(g);
}

void SequenceLayerTimeline::mouseDown(const MouseEvent &e)
{
	if (e.mods.isLeftButtonDown() && e.eventComponent->getParentComponent() == this)
	{
		item->selectThis();
	}
}

void SequenceLayerTimeline::controllableFeedbackUpdateInternal(Controllable * c)
{
	BaseItemMinimalUI::controllableFeedbackUpdateInternal(c);

	if (c == item->uiHeight)
	{
		setSize(getWidth(), item->uiHeight->intValue());
	}
	else if (c == item->sequence->viewStartTime || c == item->sequence->viewEndTime)
	{
		updateContent();
		repaint();
	}
	else if (c == item->sequence->currentTime)
	{
		repaint();
	}
}
