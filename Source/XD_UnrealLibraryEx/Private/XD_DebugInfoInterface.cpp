// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_DebugInfoInterface.h"
#include "XD_DebugFunctionLibrary.h"


// Add default functionality here for any IXD_DebugInfoInterface functions that are not pure virtual.

TSubclassOf<class UXD_DebugInfoCollector> IXD_DebugInfoInterface::GetDebugInfoCollection_Implementation() const
{
	return UXD_DebugInfoCollector::StaticClass();
}
