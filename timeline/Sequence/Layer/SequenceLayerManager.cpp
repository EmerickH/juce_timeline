#include "SequenceLayerManager.h"
/*
  ==============================================================================

    SequenceLayerManager.cpp
    Created: 28 Oct 2016 8:15:28pm
    Author:  bkupe

  ==============================================================================
*/

SequenceLayerManager::SequenceLayerManager(Sequence * _sequence) :
	BaseManager<SequenceLayer>("Layers"),
	sequence(_sequence),
	masterAudioLayer(nullptr)
{
	itemDataType = "SequenceLayer";
	skipControllableNameInAddress = true;
	hideInEditor = true;
}

SequenceLayerManager::~SequenceLayerManager()
{
	setMasterAudioLayer(nullptr);
}

SequenceLayer * SequenceLayerManager::createItem()
{
	return new SequenceLayer(sequence);
}

SequenceLayer * SequenceLayerManager::addItemFromData(var data, bool fromUndoableAction)
{
	String layerType = data.getProperty("type", "none");
	if (layerType.isEmpty()) return nullptr;
	SequenceLayer * i = SequenceLayerFactory::getInstance()->createSequenceLayer(sequence, layerType);
	if (i != nullptr) return addItem(i, data, fromUndoableAction);
	return nullptr;
}


void SequenceLayerManager::addItemInternal(SequenceLayer * item, var data)
{
	//AudioLayer * a = dynamic_cast<AudioLayer *>(item);
	/*
	if (a != nullptr)
	{
		a->addAudioLayerListener(this);
		if (a->audioModule != nullptr) updateTargetAudioLayer();
	}
	*/
}

void SequenceLayerManager::removeItemInternal(SequenceLayer * item)
{
	/*
	AudioLayer * a = dynamic_cast<AudioLayer *>(item);
	if (a != nullptr)
	{
		a->removeAudioLayerListener(this);
		if (masterAudioLayer == a)
		{
			masterAudioLayer = nullptr;
			updateTargetAudioLayer(a);
		}
	}
	*/
}

/*
void SequenceLayerManager::targetAudioModuleChanged(AudioLayer *)
{
	updateTargetAudioLayer();
}
*/

void SequenceLayerManager::updateTargetAudioLayer(AudioLayer * excludeLayer)
{
	if (masterAudioLayer == nullptr)
	{
		for (auto &i : items)
		{
			if (i == excludeLayer) continue;
			//AudioLayer * a = dynamic_cast<AudioLayer *>(i);
			/*
			if (a != nullptr && a->audioModule != nullptr)
			{
				setMasterAudioLayer(a);
				return;
			}
			*/
		}

		setMasterAudioLayer(nullptr);
	}
	else
	{
		/*
		if (masterAudioLayer->audioModule == nullptr)
		{
			DBG("master is not null but has no audioModule, setting master to null and searching");
			masterAudioLayer = nullptr;
			updateTargetAudioLayer();
		}
		else
		{
			DBG("master has changed its module");
			setMasterAudioLayer(masterAudioLayer); //force refresh
		}
		*/
	}	
}

void SequenceLayerManager::setMasterAudioLayer(AudioLayer * layer)
{
	masterAudioLayer = layer;
	//sequence->setMasterAudioModule(masterAudioLayer != nullptr ? masterAudioLayer->audioModule : nullptr);
}

