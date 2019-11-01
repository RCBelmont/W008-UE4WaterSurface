#include "ShaderTest.h"
#include "RHIResources.h"
#include "RHIStaticStates.h"
#include "PixelShaderUtils.h"

class FTestShader : public FGlobalShader
{
public:
	FTestShader()
	{
	}

	FTestShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FGlobalShader(Initializer)
	{
		SimpleCol.Bind(Initializer.ParameterMap, TEXT("SimpleColor"));
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

	// static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	// {
	// 	FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
	// 	OutEnvironment.SetDefine(TEXT("TEST_MICRO"), 1);
	// }
	//
	// 
	void SetParameters(
		FRHICommandListImmediate& RHICmdList,
		const FLinearColor& MyColor
	)
	{
		SetShaderValue(RHICmdList, GetPixelShader(), SimpleCol, MyColor);
	}

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
		Ar << SimpleCol;
		return bShaderHasOutdatedParameters;
	}

private:
	FShaderParameter SimpleCol;
};

class FMyTestVS : public FTestShader
{
DECLARE_EXPORTED_SHADER_TYPE(FMyTestVS, Global, /*MYMODULE_API*/);

	FMyTestVS()
	{
	}

	FMyTestVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FTestShader(Initializer)
	{
	}
};

class FMyTestPS : public FTestShader
{
DECLARE_EXPORTED_SHADER_TYPE(FMyTestPS, Global, /*MYMODULE_API*/);

	FMyTestPS()
	{
	}

	FMyTestPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FTestShader(Initializer)
	{
	}
};


TGlobalResource<FMyVertexDeclaration> VertexDec;
IMPLEMENT_SHADER_TYPE(, FMyTestVS, TEXT("/Plugin/WaterSurfacePlugin/TestShader.usf"), TEXT("MainVS"), SF_Vertex)
IMPLEMENT_SHADER_TYPE(, FMyTestPS, TEXT("/Plugin/WaterSurfacePlugin/TestShader.usf"), TEXT("MainPS"), SF_Pixel)


void ShaderTest::Draw_RenderThread(
	FRHICommandListImmediate& RHICmdList,
	FTextureRenderTargetResource* OutputRenderTargetResource,
	ERHIFeatureLevel::Type FeatureLevel,
	FName TextureRenderTargetName,
	TArray<FVector4> ParticleList,
	FLinearColor MyColor)
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
		TShaderMapRef<FMyTestVS> VertexShader(GlobalShaderMap);
		TShaderMapRef<FMyTestPS> PixelShader(GlobalShaderMap);
		//Set Graphic PipelineState
		FGraphicsPipelineStateInitializer GraphicsPSOInit;
		RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
		GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
		GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
		GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
		//GraphicsPSOInit.PrimitiveType = PT_TriangleList;
		GraphicsPSOInit.PrimitiveType = PT_PointList;

		GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = VertexDec.VertexDeclarationRHI;
		//GetVertexDeclarationFVector4();//GFilterVertexDeclaration.VertexDeclarationRHI;// GetVertexDeclarationFVector4();
		GraphicsPSOInit.BoundShaderState.VertexShaderRHI = GETSAFERHISHADER_VERTEX(*VertexShader);
		GraphicsPSOInit.BoundShaderState.PixelShaderRHI = GETSAFERHISHADER_PIXEL(*PixelShader);
		SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);

		FMyRenderResources RS;
		RS.SetVertexPos(ParticleList);
		TArray<uint16> IndexList;
		for (int i = 0; i < ParticleList.Num(); i++)
		{
			IndexList.Add(i);
		}
		RS.SetIndex(IndexList, IndexList.Num());

		PixelShader->SetParameters(RHICmdList, MyColor);
		FVertexBufferRHIRef VertexBufferRHI;
		RS.RHIDraw(RHICmdList, RS.GetVertexBuffer(), RS.GetIndexBuffer(), RS, 1);
	}
	RHICmdList.EndRenderPass();


	//RHICmdList.CopyToResolveTarget(RenderTargetTexture, OutputRenderTargetResource->TextureRHI, FResolveParams());
}
