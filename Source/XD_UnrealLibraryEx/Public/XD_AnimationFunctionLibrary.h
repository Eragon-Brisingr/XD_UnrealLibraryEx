// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XD_AnimationFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class XD_UNREALLIBRARYEX_API UXD_AnimationFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "游戏|动画")
	static float GetMontageGlobalWeight(class ACharacter* Character, FName SlotName);
	
	UFUNCTION(BlueprintPure, Category = "游戏|动画")
	static bool IsPlayingMontage(class ACharacter* Character, FName SlotName);
	
};
