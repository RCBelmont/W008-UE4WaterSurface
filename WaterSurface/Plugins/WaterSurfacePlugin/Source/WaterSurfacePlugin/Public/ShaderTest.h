#pragma once
#include "CoreMinimal.h"
#include "GlobalShader.h"
#include "ShaderParameterUtils.h"
#include "TextureResource.h"
#include "PipelineStateCache.h"
#include "MyRenderResources.h"


extern TGlobalResource<FMyVertexDeclaration> VertexDec;

class ShaderTest
{


public:
	static void Draw_RenderThread(
		FRHICommandListImmediate& RHICmdList,
		FTextureRenderTargetResource* OutputRenderTargetResource,
		ERHIFeatureLevel::Type FeatureLevel,
		FName TextureRenderTargetName,
		TArray<FVector4> ParticleList,
		TArray<FLinearColor> Velocity,
		FLinearColor MyColor);
};
