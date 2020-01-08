#include "WaterHeightMapShader.h"
#include "ShaderParameterUtils.h"
#include "Containers/DynamicRHIResourceArray.h"


class FWaterHeightMapCS : public FGlobalShader
{
DECLARE_SHADER_TYPE(FWaterHeightMapCS, Global);
private:
	FRWShaderParameter OutTexture;
	FRWShaderParameter TestStructuredBufferSurface;
	FShaderParameter Time;
	FShaderParameter SamPos;
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
		SamPos.Bind(Initializer.ParameterMap, TEXT("SamPos"));
		TestStructuredBufferSurface.Bind(Initializer.ParameterMap, TEXT("TestStructuredBuffer"));
	}

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParams = FGlobalShader::Serialize(Ar);
		Ar << OutTexture << Time << TestStructuredBufferSurface<< SamPos;
		return bShaderHasOutdatedParams;
	}

	void SetParams(FRHICommandListImmediate& RHICmdList, FUnorderedAccessViewRHIRef UAV, float TimeTick,
	               FUnorderedAccessViewRHIRef BuffUAV, FVector Pos)
	{
		FRHIComputeShader* ComputeShaderRHI = GetComputeShader();
		RHICmdList.SetUAVParameter(ComputeShaderRHI, OutTexture.GetUAVIndex(), UAV);
		RHICmdList.SetUAVParameter(ComputeShaderRHI, TestStructuredBufferSurface.GetUAVIndex(), BuffUAV);
		SetShaderValue(RHICmdList, GetComputeShader(), Time, TimeTick);
		SetShaderValue(RHICmdList, GetComputeShader(), SamPos, Pos);
	}

	void UnbindBuffers(FRHICommandList& RHICmdList)
	{
		FRHIComputeShader* ComputeShaderRHI = GetComputeShader();
		RHICmdList.SetUAVParameter(ComputeShaderRHI, OutTexture.GetUAVIndex(), nullptr);
		RHICmdList.SetUAVParameter(ComputeShaderRHI, TestStructuredBufferSurface.GetUAVIndex(), nullptr);
	}
};

IMPLEMENT_SHADER_TYPE(, FWaterHeightMapCS, TEXT("/Plugin/WaterPreProcess/WaterHeightMapCs.usf"), TEXT("MainCS"),
                        SF_Compute);

FVector FWaterHeightMapShader::DrawWaterHeightMap_RenderThread(
	FRHICommandListImmediate& RHICmdList,
	ERHIFeatureLevel::Type FeatureLevel,
	FTextureRenderTargetResource* OutputSurface,
	float TimeTick,
	FVector HeightSamplePos
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


	TResourceArray<TestStruct> bufferData;
	bufferData.Reset();
	for (int i = 0; i < 1; i++)
	{
		TestStruct TestElement;
		TestElement.TestPos = HeightSamplePos;
		TestElement.TestPos1 = FVector(0, 0, 0);
		bufferData.Add(TestElement);
	}

	//bufferData.Add(TestElement);
	bufferData.SetAllowCPUAccess(true);

	FRHIResourceCreateInfo TestBufferCreateInfo;
	TestBufferCreateInfo.ResourceArray = &bufferData;

	FStructuredBufferRHIRef TestStructureBuff;
	FUnorderedAccessViewRHIRef TestStructureBuffUAV;
	TestStructureBuff = RHICreateStructuredBuffer(sizeof(TestStruct), sizeof(TestStruct) * bufferData.Num(),
	                                              BUF_UnorderedAccess | BUF_ShaderResource, TestBufferCreateInfo);
	TestStructureBuffUAV = RHICreateUnorderedAccessView(TestStructureBuff, true, false);

	ComputeShader->SetParams(RHICmdList, TextureUAV, TimeTick, TestStructureBuffUAV, HeightSamplePos);


	DispatchComputeShader(RHICmdList, *ComputeShader, OutputSurface->GetSizeX() / 16, OutputSurface->GetSizeY() / 16,
	                      1);

	ComputeShader->UnbindBuffers(RHICmdList);
	RHICmdList.CopyToResolveTarget(Texture, OutputSurface->TextureRHI, FResolveParams());
	TArray<TestStruct> Data;
	Data.Reset(1);
	TestStruct TestElement1;
	Data.Add(TestElement1);
	TestStruct* ptr = (TestStruct*)RHILockStructuredBuffer(TestStructureBuff, 0, sizeof(TestStruct) * bufferData.Num(),
	                                                       EResourceLockMode::RLM_ReadOnly);
	FMemory::Memcpy(Data.GetData(), ptr, sizeof(TestStruct) * bufferData.Num());
	RHIUnlockStructuredBuffer(TestStructureBuff);

	TextureUAV.SafeRelease();
	TestStructureBuffUAV.SafeRelease();
	Texture.SafeRelease();
	TestStructureBuff.SafeRelease();
	return Data[0].TestPos1;
}
