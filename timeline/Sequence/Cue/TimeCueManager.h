/*
  ==============================================================================

    TimeCueManager.h
    Created: 6 Apr 2017 11:13:27am
    Author:  Ben

  ==============================================================================
*/

#pragma once

class TimeCueManager :
	public BaseManager<TimeCue>
{
public:
	TimeCueManager();
	~TimeCueManager();

	void addCueAt(float time);
	void reorderItems() override;

	Array<float> getAllCueTimes(float minTime = 0, float maxTime = 0, bool includeDisabled = false);
	float getNearestCueForTime(float time, bool includeDisabled = false);

	Array<TimeCue *> getCuesInTimespan(float startTime, float endTime, bool includeDisabled = false);
	
	float getNextCueForTime(float time, bool includeDisabled = false);
	float getPrevCueForTime(float time, float goToPreviousThreshold, bool includeDisabled = false);

	static int compareTime(TimeCue * t1, TimeCue * t2);

};
