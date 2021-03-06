﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_PropertyCustomizationEx.h"
#include <DetailWidgetRow.h>
#include <IDetailChildrenBuilder.h>
#include <Editor.h>
#include <Subsystems/AssetEditorSubsystem.h>
#include "XD_AssetFunctionLibrary.h"

#define LOCTEXT_NAMESPACE "XD_PropertyCustomizationEx"

void FPropertyCustomizeHelper::HeaderRowDrawProperty(class FDetailWidgetRow& HeaderRow, const TSharedPtr<IPropertyHandle>& PropertyHandle, bool bDisplayDefaultPropertyButtons /*= true*/)
{
	HeaderRow.NameContent()
		[
			PropertyHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		.MinDesiredWidth(300.f)
		[
			PropertyHandle->CreatePropertyValueWidget(bDisplayDefaultPropertyButtons)
		];
}

uint32 FPropertyCustomizeHelper::GetNumChildren(const TSharedRef<IPropertyHandle>& PropertyHandle)
{
	uint32 NumChildren;
	PropertyHandle->GetNumChildren(NumChildren);
	return NumChildren;
}

UObject* FPropertyCustomizeHelper::GetOuter(const TSharedRef<IPropertyHandle>& PropertyHandle)
{
	TArray<UObject*> Outers;
	PropertyHandle->GetOuterObjects(Outers);
	return Outers.Num() > 0 ? Outers[0] : nullptr;
}

bool FPropertyCustomizeHelper::IsInArray(const TSharedRef<IPropertyHandle>& PropertyHandle)
{
	return PropertyHandle->GetIndexInArray() != INDEX_NONE;
}

void FPropertyCustomizeHelper::StructBuilderDrawPropertys(class IDetailChildrenBuilder& StructBuilder, const TSharedRef<IPropertyHandle>& PropertyHandle, const TArray<FName>& ExcludePropertyNames, UStruct* StopShowType)
{
	for (uint32 ChildIndex = 0; ChildIndex < GetNumChildren(PropertyHandle); ++ChildIndex)
	{
		const TSharedRef<IPropertyHandle> ChildHandle = PropertyHandle->GetChildHandle(ChildIndex).ToSharedRef();

		FProperty* ChildProperty = ChildHandle->GetProperty();
		UStruct* OwnerStrcut = ChildProperty->GetOwnerStruct();
		if (StopShowType == nullptr || !OwnerStrcut->IsChildOf(StopShowType))
		{
			if (!ExcludePropertyNames.Contains(*ChildProperty->GetNameCPP()))
			{
				StructBuilder.AddProperty(ChildHandle);
			}
		}
	}
}

TSharedPtr<IPropertyHandle> FPropertyCustomizeHelper::GetPropertyHandleByName(TSharedRef<class IPropertyHandle> StructPropertyHandle, FName ChildPropertyName)
{
	return StructPropertyHandle->GetChildHandle(ChildPropertyName);
}

FPropertyAccess::Result FPropertyCustomizeHelper::SetValueFromFormattedString(const TSharedPtr<IPropertyHandle>& PropertyHandle, const FString& InVale, EPropertyValueSetFlags::Type Flags /*= EPropertyValueSetFlags::DefaultFlags*/)
{
	PropertyHandle->NotifyPreChange();
	FPropertyAccess::Result Res = PropertyHandle->SetValueFromFormattedString(InVale, Flags);
	if (Res != FPropertyAccess::Fail)
	{
		PropertyHandle->NotifyPostChange(Flags);
	}
	return Res;
}

void FPropertyCustomizeHelper::ForceSetValue(const TSharedPtr<IPropertyHandle>& PropertyHandle, const FText& Text)
{
	FString StringValue;
	FTextStringHelper::WriteToBuffer(StringValue, Text);
	SetValueFromFormattedString(PropertyHandle, StringValue);
}

void FPropertyCustomizeHelper::SetObjectValue(const TSharedPtr<IPropertyHandle>& PropertyHandle, UObject* Object)
{
	if (Object)
	{
		if (Object->GetOuter()->HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
		{
			Object->SetFlags(RF_ArchetypeObject);
		}
		SetValueFromFormattedString(PropertyHandle, Object->GetPathName());
	}
	else
	{
		SetValue<UObject*>(PropertyHandle, nullptr);
	}
}

void FPropertyTypeWithInstancedButtonHelper::CustomizeHeader(UClass* Type, const TSubclassOf<class UFactory>& Factory, const FText& ButtonName, TSharedRef<class IPropertyHandle> StructPropertyHandle, TSharedRef<class IPropertyHandle> InstancePropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	HeaderRow.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		.MinDesiredWidth(300.f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		[
			InstancePropertyHandle->CreatePropertyValueWidget()
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Right)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
				.AutoWidth()
			[
				SNew(SButton)
				.OnClicked_Lambda([=]() {
					if (UObject* Instance = FPropertyCustomizeHelper::GetValue<UObject*>(InstancePropertyHandle))
					{
						GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(Instance);
					}
					return FReply::Handled();
				})
				.IsEnabled_Lambda([=]() {
					return FPropertyCustomizeHelper::GetValue<UObject*>(InstancePropertyHandle) ? true : false;
				})
				[
					SNew(STextBlock)
					.Text(LOCTEXT("打开", "打开"))
				]
			]
			+SHorizontalBox::Slot()
				.AutoWidth()
			[
				SNew(SButton)
				.OnClicked_Lambda([=]() {
					FPropertyCustomizeHelper::SetObjectValue(InstancePropertyHandle, NewObject<UObject>(FPropertyCustomizeHelper::GetOuter(InstancePropertyHandle), Type, NAME_None, RF_ArchetypeObject | RF_DefaultSubObject));
					return FReply::Handled();
				})
				.IsEnabled_Lambda([=]() {
					if (UObject* Outer = FPropertyCustomizeHelper::GetOuter(InstancePropertyHandle))
					{
						if (Outer->HasAnyFlags(RF_ClassDefaultObject))
						{
							return false;
						}
					}
					if (UObject* Instance = FPropertyCustomizeHelper::GetValue<UObject*>(InstancePropertyHandle))
					{
						return Instance->IsAsset();
					}
					return true;
				})
				[
					SNew(STextBlock)
					.Text(ButtonName)
				]
			]
			+ SHorizontalBox::Slot()
				.AutoWidth()
			[
				SNew(SButton)
				.OnClicked_Lambda([=]() {
					if (UObject* Instance = FPropertyCustomizeHelper::GetValue<UObject*>(InstancePropertyHandle))
					{
						FPropertyCustomizeHelper::SetValue(InstancePropertyHandle, UXD_AssetFunctionLibrary::ConvertObjectToAsset(Instance, Instance->GetOuter(), Factory));
					}
					return FReply::Handled();
				})
				.IsEnabled_Lambda([=]() {
					if (Factory)
					{
						if (UObject* Instance = FPropertyCustomizeHelper::GetValue<UObject*>(InstancePropertyHandle))
						{
							return !Instance->IsAsset();
						}
					}
					return false;
				})
				[
					SNew(STextBlock)
					.Text(LOCTEXT("创建为资源", "创建为资源"))
				]
			]
		]
	];
}

#undef LOCTEXT_NAMESPACE
