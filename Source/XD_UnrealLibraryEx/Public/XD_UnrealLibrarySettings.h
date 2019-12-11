// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <UObject/NoExportTypes.h>
#include "XD_UnrealLibrarySettings.generated.h"

/**
 * 
 */
UCLASS(Config = "XD_UnrealLibrarySetting", defaultconfig)
class XD_UNREALLIBRARYEX_API UXD_UnrealLibrarySettings : public UObject
{
	GENERATED_BODY()
public:
	UXD_UnrealLibrarySettings();

	UPROPERTY(EditAnywhere, Category = "调试", Config)
	TSoftClassPtr<class UXD_DebugInfoCollector> DebugInfoCollector;

	TSubclassOf<class UXD_DebugInfoCollector> GetDebugInfoCollector() const;
};
