// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterPreprocessLib.h"
#include "Engine/World.h"
#include "SceneInterface.h"
#include "WaterHeightMapShader.h"
#include "Engine/Texture2D.h"

FVector UWaterPreprocessLib::DrawWaterHeightMap(
	AActor* Target,
	UTextureRenderTarget2D* HeightMapRT,
	float TimeTick,
	UTexture2D* AmpNoise
)
{
	FVector RetVector;
	
	UWorld* World = Target->GetWorld();
	ERHIFeatureLevel::Type FeatureLevel = World->Scene->GetFeatureLevel();
	FTextureRenderTargetResource* OutRTResource = HeightMapRT->GameThread_GetRenderTargetResource();

	ENQUEUE_RENDER_COMMAND(CaptureCommand)(
		[OutRTResource, FeatureLevel, TimeTick, RetVector, AmpNoise](
		FRHICommandListImmediate& RHICmdList)
		{
			
			
			FWaterHeightMapShader::DrawWaterHeightMap_RenderThread(
				RHICmdList,
				FeatureLevel,
				OutRTResource,
				TimeTick,
				AmpNoise->Resource->TextureRHI
			);
			//WaterEvent::GetInstance()->TestFinish.Broadcast(pos);
		});


	
	return RetVector;
}
