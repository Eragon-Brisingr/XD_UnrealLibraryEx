// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XD_TextureFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class XD_UNREALLIBRARYEX_API UXD_TextureFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "��Ϸ|����")
	static float GetPixelBrightness(const FColor& Pixel);

	UFUNCTION(BlueprintCallable, Category = "��Ϸ|����")
	static float GetPixelsMaxBrightness(const TArray<FColor>& Pixels);
};
