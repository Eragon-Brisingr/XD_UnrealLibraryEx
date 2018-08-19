// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "XD_UnrealLibraryEx.h"
#include "XD_UnrealLibrarySettings.h"
#if WITH_EDITOR
#include <ISettingsModule.h>
#include <ISettingsSection.h>
#endif

#define LOCTEXT_NAMESPACE "FXD_UnrealLibraryExModule"

void FXD_UnrealLibraryExModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

#if WITH_EDITOR
	// register settings
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

	if (SettingsModule != nullptr)
	{
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", "XD_UnrealLibrarySettings",
			LOCTEXT("XD_UnrealLibrarySettings", "XD_UnrealLibrarySettings"),
			LOCTEXT("XD_UnrealLibrarySettingsDescription", "Configure the XD_UnrealLibrarySettings plug-in."),
			GetMutableDefault<UXD_UnrealLibrarySettings>()
		);
	}
#endif //WITH_EDITOR

}

void FXD_UnrealLibraryExModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXD_UnrealLibraryExModule, XD_UnrealLibraryEx)