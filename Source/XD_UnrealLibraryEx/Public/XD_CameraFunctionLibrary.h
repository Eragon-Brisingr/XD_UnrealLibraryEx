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
	//获取当前激活的摄像机的位置，包括编辑器下
	UFUNCTION(BlueprintPure, Category = "游戏|摄像机", meta = (WorldContext = "WorldContextObject"))
	static FVector GetCameraLocation(const UObject* WorldContextObject);
	
	//获取当前激活的摄像机的旋转，包括编辑器下
	UFUNCTION(BlueprintPure, Category = "游戏|摄像机", meta = (WorldContext = "WorldContextObject"))
	static FRotator GetCameraRotation(const UObject* WorldContextObject);
};
