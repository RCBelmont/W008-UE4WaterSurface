// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Delegates/DelegateCombinations.h"
#include "WaterPreprocessLib.generated.h"

/**
 * 
 */
UCLASS()
class WATERPREPROCESS_API UWaterPreprocessLib : public UBlueprintFunctionLibrary
{
public:
	DECLARE_EVENT(UWaterPreprocessLib, FOnTestFinish)
	FOnTestFinish& OnTestFinish() { return TestFinish; }
	GENERATED_BODY()
		UFUNCTION(BlueprintCallable, Category = "WaterEffect")
		static FVector DrawWaterHeightMap(
			AActor* Target,
			UTextureRenderTarget2D* HeightMapRT,
			float TimeTick,
			UTexture2D* AmpNoise
		);
private:
	 FOnTestFinish TestFinish;
};
