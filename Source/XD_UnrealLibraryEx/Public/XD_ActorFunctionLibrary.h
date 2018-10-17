// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <Components/ActorComponent.h>
#include "XD_ActorFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class XD_UNREALLIBRARYEX_API UXD_ActorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "游戏|工具", meta = (DeterminesOutputType = "Class"))
	static UActorComponent* AddComponent(AActor* Actor, TSubclassOf<class UActorComponent> Class, FName Name = NAME_None);

	template<typename ComponentType>
	static ComponentType* AddComponent(AActor* Actor, const FName& Name = NAME_None)
	{
		return (ComponentType*)AddComponent(Actor, ComponentType::StaticClass(), Name);
	}

	

public:
	/**
	* 分帧优化，根据当前总帧数决定执不执行事件
	*
	* @param SliceCount 决定分几帧执行一次
	* @param SliceIndex 决定分分帧后第几帧执行
	*/
	UFUNCTION(BlueprintPure, Category = "游戏|时间系统")
	static bool IsInFrameSlice(int32 SliceCount = 3, int32 SliceIndex = 0);
};
