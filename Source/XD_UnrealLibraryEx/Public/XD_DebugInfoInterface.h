// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "XD_DebugInfoInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UXD_DebugInfoInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class XD_UNREALLIBRARYEX_API IXD_DebugInfoInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category = "游戏|调试")
	TSubclassOf<class UXD_DebugInfoCollector> GetDebugInfoCollection() const;
	virtual TSubclassOf<class UXD_DebugInfoCollector> GetDebugInfoCollection_Implementation() const;
};
