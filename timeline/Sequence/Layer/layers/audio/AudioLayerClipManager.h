/*
  ==============================================================================

    AudioLayerClipManager.h
    Created: 8 Feb 2017 12:19:53pm
    Author:  Ben

  ==============================================================================
*/

#ifndef AUDIOLAYERCLIPMANAGER_H_INCLUDED
#define AUDIOLAYERCLIPMANAGER_H_INCLUDED

class AudioLayerClipManager :
	public BaseManager<AudioLayerClip>
{
public:
	AudioLayerClipManager();
	~AudioLayerClipManager();

	AudioLayerClip * addClipAt(float time);

	AudioLayerClip * getClipAtTime(float time);
};

#endif  // AUDIOLAYERCLIPMANAGER_H_INCLUDED
