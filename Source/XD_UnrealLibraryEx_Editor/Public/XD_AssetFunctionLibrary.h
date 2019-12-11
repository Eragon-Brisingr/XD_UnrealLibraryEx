// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <Templates/SubclassOf.h>
#include "XD_AssetFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class XD_UNREALLIBRARYEX_EDITOR_API UXD_AssetFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static UObject* ConvertObjectToAsset(UObject* ObjectToConvert, UObject* Owner, const TSubclassOf<class UFactory>& Factory);
};
