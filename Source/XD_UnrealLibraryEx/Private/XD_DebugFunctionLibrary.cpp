// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_DebugFunctionLibrary.h"
#include "XD_DebugInfoCollector.h"
#include "XD_UnrealLibrarySettings.h"




FString UXD_DebugFunctionLibrary::GetDebugName(const UObject* Object)
{
	if (TSubclassOf<UXD_DebugInfoCollector> XD_DebugInfoCollector = GetDefault<UXD_UnrealLibrarySettings>()->DebugInfoCollector)
	{
		return XD_DebugInfoCollector.GetDefaultObject()->GetDebugName(Object);
	}
	return UXD_DebugInfoCollector::StaticClass()->GetDefaultObject<UXD_DebugInfoCollector>()->GetDebugName(Object);
}

FString UXD_DebugInfoConverter::GetDebugName_Implementation(const UObject* Object) const
{
	return Object->GetName();
}

FString UXD_DebugInfoCollector::GetDebugName(const UObject* Object)
{
	if (Object)
	{
		const UClass* NeedFindClass = Object->GetClass();
		while (NeedFindClass)
		{
			if (TSubclassOf<UXD_DebugInfoConverter>* DebugInfoConverter = TypeDebugInfoMapping.Find(NeedFindClass))
			{
				return (*DebugInfoConverter).GetDefaultObject()->GetDebugName(Object);
			}
			else
			{
				NeedFindClass = NeedFindClass->GetSuperClass();
			}
		}
		return Object->GetName();
	}
	else
	{
		return TEXT("None");
	}
}

