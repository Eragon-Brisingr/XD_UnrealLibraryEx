// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "XD_DebugInfoCollector.generated.h"

/**
 * 
 */
UCLASS(const, Blueprintable)
class UXD_DebugInfoCollector : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "调试")
	TMap<UClass*, TSubclassOf<class UXD_DebugInfoConverter>> TypeDebugInfoMapping;

	FString GetDebugName(const UObject* Object);
};


/**
* 调试信息获取的转换器，继承重写以覆盖行为
*/
UCLASS(const, Blueprintable)
class XD_UNREALLIBRARYEX_API UXD_DebugInfoConverter : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, Category = "调试")
		FString GetDebugName(const UObject* Object) const;
	virtual FString GetDebugName_Implementation(const UObject* Object) const;
};
