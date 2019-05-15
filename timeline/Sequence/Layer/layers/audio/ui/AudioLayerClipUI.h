/*
  ==============================================================================

    AudioLayerClipUI.h
    Created: 8 Feb 2017 12:20:09pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class AudioLayerClipUI :
	public BaseItemUI<AudioLayerClip>,
	public AudioLayerClip::AsyncListener
{
public:
	AudioLayerClipUI(AudioLayerClip * clip);
	~AudioLayerClipUI();

	//interaction
	float timeAtMouseDown;
	int posAtMouseDown;

	std::unique_ptr<ImageButton> browseBT;
	AudioThumbnailCache thumbnailCache;
	AudioThumbnail thumbnail;
	AudioLayerClip * clip;

	void paint(Graphics &g) override;
	void resizedInternalHeader(Rectangle<int> &r) override;

	void mouseDown(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;
	void mouseUp(const MouseEvent &e) override;

	void buttonClicked(Button * b) override;

	void controllableFeedbackUpdateInternal(Controllable *) override;

	void newMessage(const AudioLayerClip::ClipEvent &e) override;

	class ClipUIListener
	{
	public:
		virtual ~ClipUIListener() {}
		virtual void clipUITimeChanged(AudioLayerClipUI *) {}
		virtual void clipUIDragged(AudioLayerClipUI *, const MouseEvent &) {}
	};

	ListenerList<ClipUIListener> clipUIListeners;
	void addClipUIListener(ClipUIListener* newListener) { clipUIListeners.add(newListener); }
	void removeClipUIListener(ClipUIListener* listener) { clipUIListeners.remove(listener); }

};