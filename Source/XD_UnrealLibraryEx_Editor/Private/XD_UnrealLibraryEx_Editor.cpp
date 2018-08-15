// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "XD_UnrealLibraryEx_Editor.h"
#include <ISettingsModule.h>
#include <ISettingsSection.h>

#define LOCTEXT_NAMESPACE "FXD_UnrealLibraryExModule"

void FXD_UnrealLibraryEx_EditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

}

void FXD_UnrealLibraryEx_EditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXD_UnrealLibraryEx_EditorModule, XD_UnrealLibraryEx_Editor)