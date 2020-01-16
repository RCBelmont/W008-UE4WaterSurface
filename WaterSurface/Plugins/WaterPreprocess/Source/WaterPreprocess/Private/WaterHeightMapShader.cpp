#include "WaterHeightMapShader.h"
#include "ShaderParameterUtils.h"
#include "Containers/DynamicRHIResourceArray.h"
#include "RHIStaticStates.h"


class FWaterHeightMapCS : public FGlobalShader
{
DECLARE_SHADER_TYPE(FWaterHeightMapCS, Global);
private:
	FRWShaderParameter OutTexture;
	FShaderParameter Time;
	FShaderResourceParameter AmpNoiseTex;
	FShaderResourceParameter samplerAmpNoiseTex;
public:
	FWaterHeightMapCS()
	{
	}

	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters,
	                                         FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
	}

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}

	FWaterHeightMapCS(const ShaderMetaType::CompiledShaderInitializerType& Initializer): FGlobalShader(Initializer)
	{
		OutTexture.Bind(Initializer.ParameterMap, TEXT("OutTexture"));
		Time.Bind(Initializer.ParameterMap, TEXT("TimeTick"));
		AmpNoiseTex.Bind(Initializer.ParameterMap, TEXT("AmpNoise"));
		samplerAmpNoiseTex.Bind(Initializer.ParameterMap, TEXT("samplerAmpNoise"));
	}

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParams = FGlobalShader::Serialize(Ar);
		Ar << OutTexture << Time << AmpNoiseTex;
		return bShaderHasOutdatedParams;
	}

	void SetParams(FRHICommandListImmediate& RHICmdList, FUnorderedAccessViewRHIRef UAV, float TimeTick, FTextureRHIRef NoiseTex)
	{
		FRHIComputeShader* ComputeShaderRHI = GetComputeShader();
		RHICmdList.SetUAVParameter(ComputeShaderRHI, OutTexture.GetUAVIndex(), UAV);
		SetShaderValue(RHICmdList, GetComputeShader(), Time, TimeTick);
		SetTextureParameter(
			RHICmdList,
			ComputeShaderRHI,
			AmpNoiseTex,
			samplerAmpNoiseTex,
			TStaticSamplerState<SF_Bilinear, AM_Wrap, AM_Wrap, AM_Wrap>::GetRHI(),
			NoiseTex);
	}

	void UnbindBuffers(FRHICommandList& RHICmdList)
	{
		FRHIComputeShader* ComputeShaderRHI = GetComputeShader();
		RHICmdList.SetUAVParameter(ComputeShaderRHI, OutTexture.GetUAVIndex(), nullptr);
	}
};

IMPLEMENT_SHADER_TYPE(, FWaterHeightMapCS, TEXT("/Plugin/WaterPreProcess/WaterHeightMapCs.usf"), TEXT("MainCS"),
                        SF_Compute);

void FWaterHeightMapShader::DrawWaterHeightMap_RenderThread(
	FRHICommandListImmediate& RHICmdList,
	ERHIFeatureLevel::Type FeatureLevel,
	FTextureRenderTargetResource* OutputSurface,
	float TimeTick,
	FTextureRHIRef AmpNoise
)
{
	check(IsInRenderingThread());
	TShaderMapRef<FWaterHeightMapCS> ComputeShader(GetGlobalShaderMap(FeatureLevel));
	RHICmdList.SetComputeShader(ComputeShader->GetComputeShader());
	FRHIResourceCreateInfo CreateInfo;
	FTexture2DRHIRef Texture = RHICreateTexture2D(
		OutputSurface->GetSizeX(),
		OutputSurface->GetSizeY(),
		OutputSurface->GetRenderTargetTexture()->GetFormat(), 1,
		1, TexCreate_ShaderResource | TexCreate_UAV, CreateInfo);
	FUnorderedAccessViewRHIRef TextureUAV = RHICreateUnorderedAccessView(Texture);


	ComputeShader->SetParams(RHICmdList, TextureUAV, TimeTick, AmpNoise);


	DispatchComputeShader(RHICmdList, *ComputeShader, OutputSurface->GetSizeX() / 16, OutputSurface->GetSizeY() / 16,
	                      1);

	ComputeShader->UnbindBuffers(RHICmdList);
	RHICmdList.CopyToResolveTarget(Texture, OutputSurface->TextureRHI, FResolveParams());
	TextureUAV.SafeRelease();
	Texture.SafeRelease();
}
