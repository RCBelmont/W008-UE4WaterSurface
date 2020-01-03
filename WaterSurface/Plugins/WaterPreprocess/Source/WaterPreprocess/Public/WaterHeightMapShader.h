#pragma once
#include "ShaderCore.h"
#include "GlobalShader.h"
#include "TextureResource.h"
class FWaterHeightMapShader
{
public:
	static void DrawWaterHeightMap_RenderThread(
		FRHICommandListImmediate& RHICmdList,
		ERHIFeatureLevel::Type FeatureLevel,
		FTextureRenderTargetResource* OutputSurface
	);
};