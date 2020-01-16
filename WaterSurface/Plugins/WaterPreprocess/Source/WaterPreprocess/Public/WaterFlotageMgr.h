#pragma once
#include "Delegates/DelegateCombinations.h"
#include "GameFramework/Actor.h"


class FWaterFlotageMgr
{
public:
	DECLARE_EVENT(FWaterFlotageMgr, FOnWaterFlotageHeightCalcDone)
	FOnWaterFlotageHeightCalcDone& OnWaterFlotageHeightCalcDone() { return FloatageCalcDone; }


	
	static FWaterFlotageMgr* GetInstance()
	{
		if (m_instance_ptr == nullptr)
		{
			m_instance_ptr = new FWaterFlotageMgr;
		}
		return m_instance_ptr;
	}
	FOnWaterFlotageHeightCalcDone FloatageCalcDone;

	void RegisterFlotageActor(AActor* Flotage);
private:


	TArray<AActor*> FlotageActorL;

	FWaterFlotageMgr()
	{
	}

	static FWaterFlotageMgr* m_instance_ptr;
};

FWaterFlotageMgr* FWaterFlotageMgr::m_instance_ptr = nullptr;
