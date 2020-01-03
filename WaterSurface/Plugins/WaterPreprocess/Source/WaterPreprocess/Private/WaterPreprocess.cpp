// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "WaterPreprocess.h"
#include "Misc/Paths.h"
#include "Interfaces/IPluginManager.h"
#include "ShaderCore.h"
#define LOCTEXT_NAMESPACE "FWaterPreprocessModule"

void FWaterPreprocessModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("WaterPreprocess"))->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/Plugin/WaterPreProcess"), PluginShaderDir);
}

void FWaterPreprocessModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWaterPreprocessModule, WaterPreprocess)