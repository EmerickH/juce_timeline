/*
  ==============================================================================

    TimeCueUI.h
    Created: 6 Apr 2017 11:13:51am
    Author:  Ben

  ==============================================================================
*/

#ifndef TIMECUEUI_H_INCLUDED
#define TIMECUEUI_H_INCLUDED

class TimeCueUI :
	public BaseItemMinimalUI<TimeCue>
{
public:
	TimeCueUI(TimeCue *);
	~TimeCueUI();

	Path drawPath;
	//float timeAtMouseDown;
	//float posAtMouseDown;

	void paint(Graphics &g) override;
	void resized() override;

	void mouseDoubleClick(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;

	void controllableFeedbackUpdateInternal(Controllable *) override;

	class TimeCueUIListener
	{
	public:
		virtual ~TimeCueUIListener() {}
		virtual void cueTimeChanged(TimeCueUI *) {}
		virtual void cueDragged(TimeCueUI *, const MouseEvent &) {}
	};

	ListenerList<TimeCueUIListener> cueUIListeners;
	void addCueUIListener(TimeCueUIListener* newListener) { cueUIListeners.add(newListener); }
	void removeCueUIListener(TimeCueUIListener* listener) { cueUIListeners.remove(listener); }

};



#endif  // TIMECUEUI_H_INCLUDED
