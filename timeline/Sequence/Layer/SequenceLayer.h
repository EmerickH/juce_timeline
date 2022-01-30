/*
  ==============================================================================

    SequenceLayer.h
    Created: 28 Oct 2016 8:15:05pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class SequenceLayerPanel;
class SequenceLayerTimeline;

class SequenceLayer :
	public BaseItem,
	public Sequence::SequenceListener
{
public:
	SequenceLayer(Sequence * _sequence = nullptr, const String &name = "New Layer", bool _isGroup = false);
	virtual ~SequenceLayer();

	Sequence * sequence;
	bool isGroup;

	//UI
	IntParameter * uiHeight;
	ColorParameter * color;

	virtual void addDefaultContent() {}
	virtual void getAllItems(Array<SequenceLayer*> * l) { jassert(false); }

	Array<Inspectable *> selectAllItemsBetween(float start, float end);
	virtual Array<Inspectable*> selectAllItemsBetweenInternal(float start, float end);
	Array<UndoableAction*> getRemoveAllItemsBetween(float start, float end);
	virtual Array<UndoableAction*> getRemoveAllItemsBetweenInternal(float start, float end);
	Array<UndoableAction*> getInsertTimespan(float start, float length);
	virtual Array<UndoableAction*> getInsertTimespanInternal(float start, float length);
	Array<UndoableAction*> getRemoveTimespan(float start, float end);
	virtual Array<UndoableAction*> getRemoveTimespanInternal(float start, float end);

	virtual void getSnapTimes(Array<float> * arrayToFill) {}

	//Factory
	virtual String getTypeString() const override { jassert(false); return ""; } //should be overriden

	//UI
	virtual SequenceLayerPanel * getPanel();
	virtual SequenceLayerTimeline * getTimelineUI();

	//InspectableEditor * getEditorInternal(bool isRoot) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceLayer)
};