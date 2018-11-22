// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_AssetFunctionLibrary.h"
#include "Engine/Selection.h"
#include "Editor.h"
#include "AssetRegistryModule.h"
#include "Factories/Factory.h"
#include "Dialogs.h"
#include "PackageTools.h"
#include "PackageName.h"
#include "DlgPickAssetPath.h"
#include "ObjectWriter.h"
#include "ObjectReader.h"

#define LOCTEXT_NAMESPACE "XD_AssetFunctionLibrary"

UObject* UXD_AssetFunctionLibrary::ConvertObjectToAsset(UObject* ObjectToConvert, UObject* Owner, const TSubclassOf<UFactory>& Factory)
{
	FString DefaultAsset = FPackageName::GetLongPackagePath(Owner->GetOutermost()->GetName()) + TEXT("/") + ObjectToConvert->GetName();

	TSharedRef<SDlgPickAssetPath> NewCurveDlg =
		SNew(SDlgPickAssetPath)
		.Title(LOCTEXT("ConvertObjectToAsset", "选择一个文件夹创建资源"))
		.DefaultAssetPath(FText::FromString(DefaultAsset));

	if (NewCurveDlg->ShowModal() != EAppReturnType::Cancel)
	{
		FString Package(NewCurveDlg->GetFullAssetPath().ToString());
		FString Name(NewCurveDlg->GetAssetName().ToString());
		FString Group(TEXT(""));

		// Find (or create!) the desired package for this object
		UPackage* Pkg = CreatePackage(NULL, *Package);
		UPackage* OutermostPkg = Pkg->GetOutermost();

		TArray<UPackage*> TopLevelPackages;
		TopLevelPackages.Add(OutermostPkg);
		if (!UPackageTools::HandleFullyLoadingPackages(TopLevelPackages, LOCTEXT("CreateANewObject", "Create a new object")))
		{
			// User aborted.
			return nullptr;
		}

		if (!PromptUserIfExistingObject(Name, Package, Group, Pkg))
		{
			return nullptr;
		}

		// PromptUserIfExistingObject may have GCed and recreated our outermost package - re-acquire it here.
		OutermostPkg = Pkg->GetOutermost();

		// Create a new asset and set it as the external curve
		FName AssetName = *Name;
		UObject* NewObject = Factory.GetDefaultObject()->FactoryCreateNew(ObjectToConvert->GetClass(), Pkg, AssetName, RF_Public | RF_Standalone, NULL, GWarn);
		if (NewObject)
		{
			UObject* ChangeOuterObject = DuplicateObject<UObject>(ObjectToConvert, OutermostPkg);
			// serialize out the original object
			TArray<uint8> Bytes;
			FObjectWriter(ChangeOuterObject, Bytes);
			// serialize old objects on top of the new object
			FObjectReader Reader(NewObject, Bytes);

			// Set the new object as the sole selection.
			USelection* SelectionSet = GEditor->GetSelectedObjects();
			SelectionSet->DeselectAll();
			SelectionSet->Select(NewObject);

			// Notify the asset registry
			FAssetRegistryModule::AssetCreated(NewObject);

			// Mark the package dirty...
			OutermostPkg->MarkPackageDirty();

			return NewObject;
		}
	}
	return nullptr;
}

#undef LOCTEXT_NAMESPACE
