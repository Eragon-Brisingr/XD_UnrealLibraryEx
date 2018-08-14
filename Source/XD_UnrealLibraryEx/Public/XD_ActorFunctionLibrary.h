// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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

};
