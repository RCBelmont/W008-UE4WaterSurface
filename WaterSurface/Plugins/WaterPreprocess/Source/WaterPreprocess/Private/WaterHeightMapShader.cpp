#include "WaterHeightMapShader.h"



class FWaterHeightMapCS : public FGlobalShader
{
DECLARE_SHADER_TYPE(FWaterHeightMapCS, Global);
private:
	FRWShaderParameter OutTexture;
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
	}

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParams = FGlobalShader::Serialize(Ar);
		Ar << OutTexture;
		return bShaderHasOutdatedParams;
	}

	void SetParams(FRHICommandListImmediate& RHICmdList, FRHIUnorderedAccessView* UAV)
	{
		FRHIComputeShader* ComputeShaderRHI = GetComputeShader();
		RHICmdList.SetUAVParameter(ComputeShaderRHI, OutTexture.GetUAVIndex(), UAV);
	}

	void UnbindBuffers(FRHICommandList& RHICmdList)
	{
		FRHIComputeShader* ComputeShaderRHI = GetComputeShader();
		RHICmdList.SetUAVParameter(ComputeShaderRHI, OutTexture.GetUAVIndex(), nullptr);
	}
};

IMPLEMENT_SHADER_TYPE(, FWaterHeightMapCS, TEXT("/Plugin/WaterPreProcess/WaterHeightMapCs.usf"), TEXT("MainCS"), SF_Compute);

void FWaterHeightMapShader::DrawWaterHeightMap_RenderThread(
	FRHICommandListImmediate& RHICmdList,
	ERHIFeatureLevel::Type FeatureLevel,
	FTextureRenderTargetResource* OutputSurface
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
	
	ComputeShader->SetParams(RHICmdList, TextureUAV);
	
	DispatchComputeShader(RHICmdList, *ComputeShader, OutputSurface->GetSizeX() / 16, OutputSurface->GetSizeY() / 16, 1);

	ComputeShader->UnbindBuffers(RHICmdList);
	RHICmdList.CopyToResolveTarget(Texture, OutputSurface->TextureRHI, FResolveParams());

	TextureUAV.SafeRelease();
	Texture.SafeRelease();
}
