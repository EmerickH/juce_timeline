/*
  ==============================================================================

    SequenceManagerUI.cpp
    Created: 28 Oct 2016 8:13:29pm
    Author:  bkupe

  ==============================================================================
*/

SequenceManagerUI::SequenceManagerUI(const String &contentName, SequenceManager * _manager) :
	BaseManagerShapeShifterUI<SequenceManager, Sequence, SequenceUI>(contentName, _manager)
{
	addItemText = "Add Sequence";
	noItemText = "Add timed animation and control by creating a sequence";
	addExistingItems();
}

SequenceManagerUI::~SequenceManagerUI()
{
}
