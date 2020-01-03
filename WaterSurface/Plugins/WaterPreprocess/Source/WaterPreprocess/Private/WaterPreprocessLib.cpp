// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterPreprocessLib.h"
#include "Engine/World.h"
#include "SceneInterface.h"
#include "WaterHeightMapShader.h"
void UWaterPreprocessLib::DrawWaterHeightMap(AActor* Target, UTextureRenderTarget2D* HeightMapRT)
{
	
	
	UWorld* World = Target->GetWorld();
	ERHIFeatureLevel::Type FeatureLevel = World->Scene->GetFeatureLevel();
	FTextureRenderTargetResource* OutRTResource = HeightMapRT->GameThread_GetRenderTargetResource();
	
	ENQUEUE_RENDER_COMMAND(CaptureCommand)(
		[OutRTResource, FeatureLevel](
			FRHICommandListImmediate& RHICmdList)
		{
			FWaterHeightMapShader::DrawWaterHeightMap_RenderThread(
				RHICmdList,
				FeatureLevel,
				OutRTResource
			);
		});
}
