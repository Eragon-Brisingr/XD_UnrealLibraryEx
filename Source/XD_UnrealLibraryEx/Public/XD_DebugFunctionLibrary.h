// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Kismet/BlueprintFunctionLibrary.h>
#include "XD_DebugFunctionLibrary.generated.h"

/**
 * 调试信息获取的帮助函数库
 */
UCLASS()
class XD_UNREALLIBRARYEX_API UXD_DebugFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "游戏|调试")
	static FString GetDebugName(const UObject* Object);
};
