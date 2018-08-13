// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
 //通用
DECLARE_LOG_CATEGORY_EXTERN(XD_UnrealLibraryEx_Log, Log, All);
#define UnrealLibraryEx_Display_Log(Format, ...) UE_LOG(XD_UnrealLibraryEx_Log, Log, TEXT(Format), ##__VA_ARGS__)
#define UnrealLibraryEx_Warning_LOG(Format, ...) UE_LOG(XD_UnrealLibraryEx_Log, Warning, TEXT(Format), ##__VA_ARGS__)
#define UnrealLibraryEx_Error_Log(Format, ...) UE_LOG(XD_UnrealLibraryEx_Log, Error, TEXT(Format), ##__VA_ARGS__)

