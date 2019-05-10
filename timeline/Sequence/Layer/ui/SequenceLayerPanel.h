/*
  ==============================================================================

    SequenceLayerPanel.h
    Created: 17 Nov 2016 7:59:08pm
    Author:  Ben Kuper

  ==============================================================================
*/

#pragma once

class SequenceLayerPanel :
	public BaseItemUI<SequenceLayer>
{
public:
	SequenceLayerPanel(SequenceLayer *);
	virtual ~SequenceLayerPanel();

	ScopedPointer<ColorParameterUI> colorUI;
	void paintOverChildren(Graphics &g) override;
	void resized() override;
	void resizedInternalHeader(Rectangle<int> &r) override;
	
	void controllableFeedbackUpdateInternal(Controllable *) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceLayerPanel)
};