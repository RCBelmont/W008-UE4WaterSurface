#include "WaterFlotageMgr.h"

void FWaterFlotageMgr::RegisterFlotageActor(AActor* FlotageActor)
{
	int idx = FlotageActorL.Find(FlotageActor);
	UE_LOG(LogTemp, Warning, TEXT("%d"), idx);
	
	
}