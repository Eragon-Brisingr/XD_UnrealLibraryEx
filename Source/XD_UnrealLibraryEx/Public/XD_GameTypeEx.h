// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "XD_GameTypeEx.generated.h"

/**
 * 
 */

#define RF_XD_WasLoaded ((EObjectFlags)(RF_ClassDefaultObject | RF_ArchetypeObject | RF_DefaultSubObject | RF_LoadCompleted))


//Delegate Handle的蓝图包装  
USTRUCT(BlueprintType)
struct FBPDelegateHandle
{
	GENERATED_BODY()
public:
	FBPDelegateHandle() = default;
	FBPDelegateHandle(FDelegateHandle DelegateHandle)
		:DelegateHandle(DelegateHandle)
	{}

	FDelegateHandle DelegateHandle;

	friend uint32 GetTypeHash(const FBPDelegateHandle& BPDelegateHandle) { return GetTypeHash(BPDelegateHandle.DelegateHandle); }

	friend bool operator==(const FBPDelegateHandle& LHS, const FBPDelegateHandle& RHS) { return LHS.DelegateHandle == RHS.DelegateHandle; }
};

