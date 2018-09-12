// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XD_MathFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class XD_UNREALLIBRARYEX_API UXD_MathFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "游戏|数学")
	static float FInterpTo_Advance(float Current, float Target, float DeltaTime, float IncreaseSpeed, float DecreaseSpeed);
	
	UFUNCTION(BlueprintPure, Category = "游戏|数学")
	static float FInterpConstantTo_Advance(float Current, float Target, float DeltaTime, float IncreaseSpeed, float DecreaseSpeed);
};
