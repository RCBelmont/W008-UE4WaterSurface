// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterPreprocessLib.h"
#include "Engine/World.h"
#include "SceneInterface.h"
#include "WaterHeightMapShader.h"
#include "WaterEvent.h"

FVector UWaterPreprocessLib::DrawWaterHeightMap(
	AActor* Target,
	UTextureRenderTarget2D* HeightMapRT,
	float TimeTick,
	AActor* Target1
)
{
	FVector RetVector;

	UWorld* World = Target->GetWorld();
	ERHIFeatureLevel::Type FeatureLevel = World->Scene->GetFeatureLevel();
	FTextureRenderTargetResource* OutRTResource = HeightMapRT->GameThread_GetRenderTargetResource();

	ENQUEUE_RENDER_COMMAND(CaptureCommand)(
		[OutRTResource, FeatureLevel, TimeTick, Target1, RetVector](
		FRHICommandListImmediate& RHICmdList)
		{
			FVector HeightSamplePos = Target1->GetTransform().GetLocation();
			FVector pos = FWaterHeightMapShader::DrawWaterHeightMap_RenderThread(
				RHICmdList,
				FeatureLevel,
				OutRTResource,
				TimeTick,
				HeightSamplePos
			);
			WaterEvent::GetInstance()->TestFinish.Broadcast(pos);
		});


	
	return RetVector;
}
