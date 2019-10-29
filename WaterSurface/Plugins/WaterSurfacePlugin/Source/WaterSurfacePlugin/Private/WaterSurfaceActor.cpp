// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterSurfaceActor.h"
#include "WaterSurfacePlugin.h"
#include "Engine/Engine.h"
#include "ShaderTest.h"
#include "SceneInterface.h"

// Sets default values
AWaterSurfaceActor::AWaterSurfaceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWaterSurfaceActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaterSurfaceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AWaterSurfaceActor::Draw(UTextureRenderTarget2D* OutputRT, FLinearColor SimpleColor)
{
	check(IsInGameThread());
	if (OutputRT == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("OutputRT is Null"));
		return;
	}
	
	FTextureRenderTargetResource* TextureRenderTargetResource = OutputRT->GameThread_GetRenderTargetResource();
	UWorld* World = this->GetWorld();
	ERHIFeatureLevel::Type FeatureLevel = World->Scene->GetFeatureLevel();
	FName TextureRenderTargetName = OutputRT->GetFName();
	ENQUEUE_RENDER_COMMAND(CaptureCommand)(
		[TextureRenderTargetResource, FeatureLevel, SimpleColor, TextureRenderTargetName](FRHICommandListImmediate& RHICmdList)
		{
			ShaderTest::Draw_RenderThread(RHICmdList, TextureRenderTargetResource, FeatureLevel, TextureRenderTargetName, SimpleColor);
		}
	);
}
