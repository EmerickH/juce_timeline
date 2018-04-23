/*
  ==============================================================================

    SequenceUI.h
    Created: 28 Oct 2016 8:13:34pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SEQUENCEUI_H_INCLUDED
#define SEQUENCEUI_H_INCLUDED

class SequenceUI :
	public BaseItemUI<Sequence>,
	public Sequence::SequenceListener
{
public:
	SequenceUI(Sequence * output);
	virtual ~SequenceUI();

	ScopedPointer<FloatSliderUI> timeUI;

	void resizedInternalContent(Rectangle<int> &r) override;

	void controllableFeedbackUpdateInternal(Controllable *) override;

	JUCE_DECLARE_NON_COPYABLE(SequenceUI)
};






#endif  // SEQUENCEUI_H_INCLUDED
