/*
  ==============================================================================

    SequenceLayerPanel.cpp
    Created: 17 Nov 2016 7:59:08pm
    Author:  Ben Kuper

  ==============================================================================
*/

SequenceLayerPanel::SequenceLayerPanel(SequenceLayer * layer) :
	BaseItemUI<SequenceLayer>(layer,Direction::VERTICAL)
{
	setWantsKeyboardFocus(false); 
	setMouseClickGrabsKeyboardFocus(false);
	bringToFrontOnSelect = false;

	bgColor = item->color->getColor(); 
	colorUI.reset(item->color->createColorParamUI());

	addAndMakeVisible(colorUI.get());
	setSize(100, item->uiHeight->intValue());
}

SequenceLayerPanel::~SequenceLayerPanel()
{

}

void SequenceLayerPanel::paintOverChildren(Graphics & g)
{
	g.setColour(item->color->getColor());
	g.drawRoundedRectangle(getLocalBounds().reduced(1).toFloat(), 2, 2);
	BaseItemUI::paintOverChildren(g);
}

void SequenceLayerPanel::resized()
{
	BaseItemUI::resized();
	item->uiHeight->setValue(getHeight());
}

void SequenceLayerPanel::resizedInternalHeader(Rectangle<int>& r)
{
	colorUI->setBounds(r.removeFromRight(r.getHeight()).reduced(2));
}

void SequenceLayerPanel::controllableFeedbackUpdateInternal(Controllable * c)
{
	BaseItemMinimalUI::controllableFeedbackUpdateInternal(c);

	if (c == item->uiHeight)
	{
		if (!item->isCurrentlyLoadingData) setSize(getWidth(), item->uiHeight->intValue());
	}
	else if (c == item->color)
	{
		if (!item->isCurrentlyLoadingData) repaint();
	}

}
