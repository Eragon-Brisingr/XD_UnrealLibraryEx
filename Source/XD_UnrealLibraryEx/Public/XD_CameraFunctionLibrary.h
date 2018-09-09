// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XD_CameraFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class XD_UNREALLIBRARYEX_API UXD_CameraFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "游戏|摄像机", meta = (WorldContext = "WorldContextObject"))
	static FVector GetCameraLocation(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "游戏|摄像机", meta = (WorldContext = "WorldContextObject"))
	static FRotator GetCameraRotation(const UObject* WorldContextObject);
};
