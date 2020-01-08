#pragma once
#include "Delegates/DelegateCombinations.h"

class WaterEvent
{
public:
	DECLARE_EVENT(WaterEvent, FOnTestFinish)
	FOnTestFinish& OnTestFinish() { return TestFinish; }
	static WaterEvent* GetInstance()
	{
		if (m_instance_ptr == nullptr)
		{
			m_instance_ptr = new WaterEvent;
		}
		return m_instance_ptr;
	}
	FOnTestFinish TestFinish;
private:
	

	WaterEvent()
	{
	}

	static WaterEvent* m_instance_ptr;
};
WaterEvent* WaterEvent::m_instance_ptr = nullptr;
