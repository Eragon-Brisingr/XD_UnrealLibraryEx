// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_UnrealLibrarySettings.h"
#include "XD_DebugInfoCollector.h"




UXD_UnrealLibrarySettings::UXD_UnrealLibrarySettings()
	:DebugInfoCollector(UXD_DebugInfoCollector::StaticClass())
{

}

TSubclassOf<class UXD_DebugInfoCollector> UXD_UnrealLibrarySettings::GetDebugInfoCollector() const
{
	return DebugInfoCollector.LoadSynchronous();
}
