#pragma once
#include "ShaderCore.h"
#include "GlobalShader.h"
#include "TextureResource.h"
struct TestStruct
{
	FVector TestPos;
	FVector TestPos1;
};
class FWaterHeightMapShader
{
public:
	static FVector DrawWaterHeightMap_RenderThread(
		FRHICommandListImmediate& RHICmdList,
		ERHIFeatureLevel::Type FeatureLevel,
		FTextureRenderTargetResource* OutputSurface,
		float TimeTick,
		FVector HeightSamplePos
	);
		
};