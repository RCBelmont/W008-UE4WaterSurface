#pragma once
#include "RHIResources.h"
#include "CoreMinimal.h"
#include "RenderResource.h"
#include "PipelineStateCache.h"
#include "DynamicRHIResourceArray.h"
#include "UnrealTemplate.h"


struct FMyVertex {
	FVector4 Position;
};
class FMyVertexDeclaration : public FRenderResource
{
public:
	FVertexDeclarationRHIRef VertexDeclarationRHI;
	virtual void InitRHI() override
	{
		FVertexDeclarationElementList Elements;
		uint32 Stride = sizeof(FMyVertex);
		Elements.Add(FVertexElement(0, STRUCT_OFFSET(FMyVertex, Position), VET_Float4, 0, Stride));
		//Elements.Add(FVertexElement(0, STRUCT_OFFSET(FFilterVertex, UV), VET_Float2, 1, Stride));
		VertexDeclarationRHI = PipelineStateCache::GetOrCreateVertexDeclaration(Elements);
	}
	virtual void ReleaseRHI() override
	{
		VertexDeclarationRHI.SafeRelease();
	}

};

class FMyRenderResources
{
private:
	TResourceArray<FMyVertex, VERTEXBUFFER_ALIGNMENT> Vertices;
	TResourceArray<uint16, INDEXBUFFER_ALIGNMENT> IndexBuffer;
	uint32 PrimitiveCount;
public:
	void SetVertexPos(TArray<FVector4> PosList);
	void SetIndex(TArray<uint16> IndexList, uint32 PrimCount);
	FVertexBufferRHIRef GetVertexBuffer();
	FIndexBufferRHIRef GetIndexBuffer();
	static void RHIDraw(FRHICommandListImmediate& RHICmdList, 
		FVertexBufferRHIRef VertexBuffer, 
		FIndexBufferRHIRef IndexBuffer, 
		FMyRenderResources RResource,int InstanceCount = 1);
	uint32 GetPrimitiveCount() const;
};
