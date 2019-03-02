// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_DebugInfoCollector.h"

UXD_DebugInfoCollector::UXD_DebugInfoCollector()
{
	TypeDebugInfoMapping.Add(UObject::StaticClass(), UObjectDebugInfoConverter::StaticClass());
	TypeDebugInfoMapping.Add(UClass::StaticClass(), UClassDebugInfoConverter::StaticClass());
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

FString UObjectDebugInfoConverter::GetDebugName(const UObject* Object) const
{
	return FString::Printf(TEXT("[%s]"), *Object->GetName());
}

FString UClassDebugInfoConverter::GetDebugName(const UObject* Object) const
{
	const UClass* Class = CastChecked<UClass>(Object);
#if WITH_EDITOR
	return FString::Printf(TEXT("[%s]"), *Class->GetDisplayNameText().ToString());
#else
	return FString::Printf(TEXT("[%s]"), *Class->GetName().ToString());
#endif
}
