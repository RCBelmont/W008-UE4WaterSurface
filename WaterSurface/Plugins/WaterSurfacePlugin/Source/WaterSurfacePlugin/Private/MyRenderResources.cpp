#include "MyRenderResources.h"

void FMyRenderResources::SetVertexPos(TArray<FVector4> PosList)
{
	Vertices.Empty();
	const int PostLength = PosList.Num();
	if (PostLength <= 0)
	{
		return;
	}
	Vertices.SetNumUninitialized(PostLength);
	for (int i = 0; i < PostLength; i++)
	{
		Vertices[i].Position = PosList[i];
	}
}

void FMyRenderResources::SetIndex(TArray<uint16> IndexList, uint32 PrimCount)
{
	uint32 NumIndices = IndexList.Num();
	PrimitiveCount = PrimCount;
	IndexBuffer.AddUninitialized(NumIndices);
	FMemory::Memcpy(IndexBuffer.GetData(), IndexList.GetData(), NumIndices * sizeof(uint16));

	
}

FVertexBufferRHIRef FMyRenderResources::GetVertexBuffer()
{
	FRHIResourceCreateInfo CreateInfo(&Vertices);
	return RHICreateVertexBuffer(Vertices.GetResourceDataSize(), BUF_Static, CreateInfo);
}

FIndexBufferRHIRef FMyRenderResources::GetIndexBuffer()
{
	FRHIResourceCreateInfo CreateInfo(&IndexBuffer);
	return RHICreateIndexBuffer(sizeof(uint16), IndexBuffer.GetResourceDataSize(), BUF_Static, CreateInfo);	
}

void FMyRenderResources::RHIDraw(FRHICommandListImmediate& RHICmdList, FVertexBufferRHIRef VertexBuffer,
	FIndexBufferRHIRef IndexBuffer, FMyRenderResources RResource, int InstanceCount)
{
	RHICmdList.SetStreamSource(0, VertexBuffer, 0);

	RHICmdList.DrawIndexedPrimitive(
		IndexBuffer,
		/*BaseVertexIndex=*/ 0,
		/*MinIndex=*/ 0,
		/*NumVertices=*/ RResource.Vertices.Num(),
		/*StartIndex=*/ 0,
		/*NumPrimitives=*/ RResource.GetPrimitiveCount(),
		/*NumInstances=*/ InstanceCount);
}

uint32 FMyRenderResources::GetPrimitiveCount() const
{
	return PrimitiveCount;
}
