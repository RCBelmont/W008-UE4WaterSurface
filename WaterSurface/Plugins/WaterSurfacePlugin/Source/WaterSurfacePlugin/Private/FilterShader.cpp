#include "FilterShader.h"
#include "CommonRenderResources.h"
#include "PixelShaderUtils.h"
class FFilterShader : public FGlobalShader
{
private:
	FShaderResourceParameter ParticleTexVal;
	FShaderResourceParameter ParticleTexSampler;
	FShaderParameter TextureSize;
public:
	FFilterShader()
	{
	}

	FFilterShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FGlobalShader(Initializer)
	{
		ParticleTexVal.Bind(Initializer.ParameterMap, TEXT("ParticleTex"));
		ParticleTexSampler.Bind(Initializer.ParameterMap, TEXT("ParticleTexSampler"));
		TextureSize.Bind(Initializer.ParameterMap, TEXT("TextureSize"));
	}

	static bool ShouldCache(EShaderPlatform Platform)
	{
		return true;
	}

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		//return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM4);  
		return true;
	}

	void SetParameters(FRHICommandListImmediate& RHICmdList, const FTextureResource* Texture, int TexSize)
	{
		SetTextureParameter(
			RHICmdList,
			GetPixelShader(),
			ParticleTexVal,
			ParticleTexSampler,
			TStaticSamplerState<SF_Bilinear, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI(),
			Texture->TextureRHI);

		SetShaderValue(RHICmdList, GetPixelShader(), TextureSize, TexSize);
	}

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
		Ar << ParticleTexVal << ParticleTexSampler;
		return bShaderHasOutdatedParameters;
	}
};

class FFilterVs : public FFilterShader
{
DECLARE_EXPORTED_SHADER_TYPE(FFilterVs, Global, /*MYMODULE_API*/);

	FFilterVs()
	{
	}

	FFilterVs(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FFilterShader(Initializer)
	{
	}
};


class FFilterPs : public FFilterShader
{
	DECLARE_EXPORTED_SHADER_TYPE(FFilterPs, Global, /*MYMODULE_API*/);

	FFilterPs()
	{
	}

	FFilterPs(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FFilterShader(Initializer)
	{
	}
};

class FFilterPsV : public FFilterShader
{
	DECLARE_EXPORTED_SHADER_TYPE(FFilterPsV, Global, /*MYMODULE_API*/);

	FFilterPsV()
	{
	}

	FFilterPsV(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FFilterShader(Initializer)
	{
	}
};


IMPLEMENT_SHADER_TYPE(, FFilterVs, TEXT("/Plugin/WaterSurfacePlugin/HeightFilter.usf"), TEXT("MainVS"), SF_Vertex)
IMPLEMENT_SHADER_TYPE(, FFilterPs, TEXT("/Plugin/WaterSurfacePlugin/HeightFilter.usf"), TEXT("MainPS"), SF_Pixel)
IMPLEMENT_SHADER_TYPE(, FFilterPsV, TEXT("/Plugin/WaterSurfacePlugin/HeightFilter.usf"), TEXT("MainPSV"), SF_Pixel)


void FFilterShader_Render::Draw_RenderThread(
	FRHICommandListImmediate& RHICmdList,
	FTextureRenderTargetResource* OutputRenderTargetResource,
	ERHIFeatureLevel::Type FeatureLevel,
	FName TextureRenderTargetName,
	FTextureResource* Texture)
{
	//GFilterVertexDeclaration.VertexDeclarationRHI
	check(IsInRenderingThread());
	FRHITexture2D* RenderTargetTexture = OutputRenderTargetResource->GetRenderTargetTexture();
	RHICmdList.TransitionResource(EResourceTransitionAccess::EWritable, RenderTargetTexture);
	FRHIRenderPassInfo RPInfo(RenderTargetTexture, ERenderTargetActions::Load_Store,
		OutputRenderTargetResource->TextureRHI);
	RHICmdList.BeginRenderPass(RPInfo, TEXT("DrawTestShader"));
	{
		//Get Shader
		TShaderMap<FGlobalShaderType>* GlobalShaderMap = GetGlobalShaderMap(FeatureLevel);
		TShaderMapRef<FFilterVs> VertexShader(GlobalShaderMap);
		TShaderMapRef<FFilterPs> PixelShader(GlobalShaderMap);
		//Set Graphic PipelineState
		FGraphicsPipelineStateInitializer GraphicsPSOInit;
		RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
		GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
		GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
		GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
		GraphicsPSOInit.PrimitiveType = PT_TriangleList;
		//GraphicsPSOInit.PrimitiveType = PT_PointList;

		GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
		//GetVertexDeclarationFVector4();//GFilterVertexDeclaration.VertexDeclarationRHI;// GetVertexDeclarationFVector4();
		GraphicsPSOInit.BoundShaderState.VertexShaderRHI = GETSAFERHISHADER_VERTEX(*VertexShader);
		GraphicsPSOInit.BoundShaderState.PixelShaderRHI = GETSAFERHISHADER_PIXEL(*PixelShader);
		SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);

		PixelShader->SetParameters(RHICmdList, Texture, 256);

		FPixelShaderUtils::DrawFullscreenQuad(RHICmdList, 1);
		
	}
	RHICmdList.EndRenderPass();


	//RHICmdList.CopyToResolveTarget(RenderTargetTexture, OutputRenderTargetResource->TextureRHI, FResolveParams());
}


void FFilterShader_Render::Draw_RenderThreadV(
	FRHICommandListImmediate& RHICmdList,
	FTextureRenderTargetResource* OutputRenderTargetResource,
	ERHIFeatureLevel::Type FeatureLevel,
	FName TextureRenderTargetName,
	FTextureResource* Texture)
{
	//GFilterVertexDeclaration.VertexDeclarationRHI
	check(IsInRenderingThread());
	FRHITexture2D* RenderTargetTexture = OutputRenderTargetResource->GetRenderTargetTexture();
	RHICmdList.TransitionResource(EResourceTransitionAccess::EWritable, RenderTargetTexture);
	FRHIRenderPassInfo RPInfo(RenderTargetTexture, ERenderTargetActions::Load_Store,
		OutputRenderTargetResource->TextureRHI);
	RHICmdList.BeginRenderPass(RPInfo, TEXT("DrawTestShader"));
	{
		//Get Shader
		TShaderMap<FGlobalShaderType>* GlobalShaderMap = GetGlobalShaderMap(FeatureLevel);
		TShaderMapRef<FFilterVs> VertexShader(GlobalShaderMap);
		TShaderMapRef<FFilterPsV> PixelShader(GlobalShaderMap);
		//Set Graphic PipelineState
		FGraphicsPipelineStateInitializer GraphicsPSOInit;
		RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
		GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
		GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
		GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
		GraphicsPSOInit.PrimitiveType = PT_TriangleList;
		//GraphicsPSOInit.PrimitiveType = PT_PointList;

		GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
		//GetVertexDeclarationFVector4();//GFilterVertexDeclaration.VertexDeclarationRHI;// GetVertexDeclarationFVector4();
		GraphicsPSOInit.BoundShaderState.VertexShaderRHI = GETSAFERHISHADER_VERTEX(*VertexShader);
		GraphicsPSOInit.BoundShaderState.PixelShaderRHI = GETSAFERHISHADER_PIXEL(*PixelShader);
		SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);

		PixelShader->SetParameters(RHICmdList, Texture, 256);

		FPixelShaderUtils::DrawFullscreenQuad(RHICmdList, 1);

	}
	RHICmdList.EndRenderPass();


	//RHICmdList.CopyToResolveTarget(RenderTargetTexture, OutputRenderTargetResource->TextureRHI, FResolveParams());
}

