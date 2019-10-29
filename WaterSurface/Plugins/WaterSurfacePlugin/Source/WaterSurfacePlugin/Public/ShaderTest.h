#pragma once
#include "CoreMinimal.h"
#include "GlobalShader.h"
#include "ShaderParameterUtils.h"
#include "TextureResource.h"


class ShaderTest
{
public:
	static void Draw_RenderThread(
		FRHICommandListImmediate& RHICmdList,
		FTextureRenderTargetResource* OutputRenderTargetResource,
		ERHIFeatureLevel::Type FeatureLevel,
		FName TextureRenderTargetName,
		FLinearColor MyColor);
};
