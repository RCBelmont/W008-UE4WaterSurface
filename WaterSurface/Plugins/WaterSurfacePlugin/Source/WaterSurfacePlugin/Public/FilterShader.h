#pragma once

#include "GlobalShader.h"
#include "ShaderParameterUtils.h"
#include "RHIStaticStates.h"
#include "TextureResource.h"
#include "Shader.h"
#include "MyRenderResources.h"

class FFilterShader_Render
{
public:
	void static Draw_RenderThread(
		FRHICommandListImmediate& RHICmdList,
		FTextureRenderTargetResource* OutputRenderTargetResource,
		ERHIFeatureLevel::Type FeatureLevel,
		FName TextureRenderTargetName,
		FTextureResource* Texture);
	void static Draw_RenderThreadV(
		FRHICommandListImmediate& RHICmdList,
		FTextureRenderTargetResource* OutputRenderTargetResource,
		ERHIFeatureLevel::Type FeatureLevel,
		FName TextureRenderTargetName,
		FTextureResource* Texture);
};
