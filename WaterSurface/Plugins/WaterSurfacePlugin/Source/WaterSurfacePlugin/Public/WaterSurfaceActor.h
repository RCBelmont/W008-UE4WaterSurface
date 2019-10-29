// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "WaterSurfaceActor.generated.h"
class UTextureRenderTarget2D;
UCLASS()
class WATERSURFACEPLUGIN_API AWaterSurfaceActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWaterSurfaceActor();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crane Controls")
	UTextureRenderTarget2D* WaveParticleRT;
	UFUNCTION(BlueprintCallable, Category = Test)
	void Draw(UTextureRenderTarget2D* OutputRT,FLinearColor SimpleColor);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
