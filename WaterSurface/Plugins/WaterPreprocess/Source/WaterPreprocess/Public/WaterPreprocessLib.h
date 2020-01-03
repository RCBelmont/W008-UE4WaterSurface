// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "WaterPreprocessLib.generated.h"

/**
 * 
 */
UCLASS()
class WATERPREPROCESS_API UWaterPreprocessLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		UFUNCTION(BlueprintCallable, Category = "WaterEffect")
		static void DrawWaterHeightMap(AActor* Target, UTextureRenderTarget2D* HeightMapRT);
};
