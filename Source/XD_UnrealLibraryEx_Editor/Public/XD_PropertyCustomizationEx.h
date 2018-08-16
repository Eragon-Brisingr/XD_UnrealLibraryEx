﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XD_MacrosLibrary.h"
#include <PropertyEditorModule.h>
#include <PropertyHandle.h>

/**
 * 
 */

 //使用方法：
 //继承IPropertyTypeCustomizationMakeInstanceable
 //struct FItem_Customization : public IPropertyTypeCustomizationMakeInstanceable<FItem_Customization>
 //
 //模块中注册
 //前置使用 FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
 //RegisterCustomProperty(struct FItem, FItem_Customization);
template<typename T>
struct IPropertyTypeCustomizationMakeInstanceable : public IPropertyTypeCustomization
{
	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShareable(new T);
	}
};

#define RegisterCustomProperty(TargetType, CustomizationType) PropertyModule.RegisterCustomPropertyTypeLayout(GET_TYPE_NAME_CHECKED(TargetType), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&CustomizationType::MakeInstance))

struct XD_UNREALLIBRARYEX_EDITOR_API FPropertyCustomizeHelper
{
	static void HeaderRowDrawProperty(class FDetailWidgetRow& HeaderRow, const TSharedPtr<IPropertyHandle>& PropertyHandle, bool bDisplayDefaultPropertyButtons = true);

	static uint32 GetNumChildren(const TSharedRef<IPropertyHandle>& PropertyHandle);

	static UObject* GetOuter(const TSharedRef<IPropertyHandle>& PropertyHandle);

	static bool IsInArray(const TSharedRef<IPropertyHandle>& PropertyHandle);

	static void StructBuilderDrawPropertys(class IDetailChildrenBuilder& StructBuilder, const TSharedRef<IPropertyHandle>& PropertyHandle, const TArray<FString>& ExcludePropertyNames = {});

	static TSharedPtr<IPropertyHandle> GetPropertyHandleByName(TSharedRef<class IPropertyHandle> StructPropertyHandle, FString ChildPropertyName);

	template<typename ValueType>
	static ValueType GetValue(const TSharedPtr<IPropertyHandle>& PropertyHandle)
	{
		if (UObject* Outer = GetOuter(PropertyHandle.ToSharedRef()))
		{
			return *reinterpret_cast<ValueType*>(PropertyHandle->GetValueBaseAddress(reinterpret_cast<uint8*>(Outer)));
		}
		else
		{
			return {};
		}
	}

	template<typename ValueType>
	static ValueType& Value(const TSharedPtr<IPropertyHandle>& PropertyHandle)
	{
		return *reinterpret_cast<ValueType*>(PropertyHandle->GetValueBaseAddress(reinterpret_cast<uint8*>(GetOuter(PropertyHandle.ToSharedRef()))));
	}

	static FPropertyAccess::Result SetValueFromFormattedString(const TSharedPtr<IPropertyHandle>& PropertyHandle, const FString& InVale, EPropertyValueSetFlags::Type Flags = EPropertyValueSetFlags::DefaultFlags);

	static void ForceSetValue(const TSharedPtr<IPropertyHandle>& PropertyHandle, const FText& Text);

	template<typename StructType>
	static void SetStructValue(const TSharedPtr<IPropertyHandle>& PropertyHandle, const StructType& Value)
	{
		FString ExportString;
		StructType::StaticStruct()->ExportText(ExportString, &Value, nullptr, nullptr, 0, nullptr);
		SetValueFromFormattedString(PropertyHandle, ExportString);
	}

	static void SetObjectValue(const TSharedPtr<IPropertyHandle>& PropertyHandle, UObject* Object);
};

struct XD_UNREALLIBRARYEX_EDITOR_API IPropertyTypeWithInstancedButtonHelper
{
	static void CustomizeHeader(UClass* Type, const FText& ButtonName, TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils);
};
