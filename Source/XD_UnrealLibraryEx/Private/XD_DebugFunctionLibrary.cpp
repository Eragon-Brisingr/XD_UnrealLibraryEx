// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_DebugFunctionLibrary.h"
#include "XD_DebugInfoInterface.h"
#include "XD_DebugInfoCollector.h"




FString UXD_DebugFunctionLibrary::GetDebugName(const UObject* Object)
{
	if (GEngine && GEngine->GameSingleton && GEngine->GameSingleton->Implements<UXD_DebugInfoInterface>())
	{
		if (TSubclassOf<UXD_DebugInfoCollector> DebugInfoCollection = IXD_DebugInfoInterface::Execute_GetDebugInfoCollection(GEngine->GameSingleton))
		{
			return DebugInfoCollection->GetDefaultObject<UXD_DebugInfoCollector>()->GetDebugName(Object);
		}
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
				return (*DebugInfoConverter)->GetDefaultObject<UXD_DebugInfoConverter>()->GetDebugName(Object);
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

