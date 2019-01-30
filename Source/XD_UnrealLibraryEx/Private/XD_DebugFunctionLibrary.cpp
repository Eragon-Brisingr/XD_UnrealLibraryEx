// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_DebugFunctionLibrary.h"
#include "XD_DebugInfoCollector.h"
#include "XD_UnrealLibrarySettings.h"




FString UXD_DebugFunctionLibrary::GetDebugName(const UObject* Object)
{
	if (TSubclassOf<UXD_DebugInfoCollector> XD_DebugInfoCollector = GetDefault<UXD_UnrealLibrarySettings>()->GetDebugInfoCollector())
	{
		return XD_DebugInfoCollector.GetDefaultObject()->GetDebugName(Object);
	}
	return UXD_DebugInfoCollector::StaticClass()->GetDefaultObject<UXD_DebugInfoCollector>()->GetDebugName(Object);
}
