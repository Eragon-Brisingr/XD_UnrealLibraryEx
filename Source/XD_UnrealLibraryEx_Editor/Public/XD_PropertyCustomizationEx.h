// Fill out your copyright notice in the Description page of Project Settings.

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
#define UnregisterCustomProperty(TargetType) PropertyModule.UnregisterCustomPropertyTypeLayout(GET_TYPE_NAME_CHECKED(TargetType))

struct XD_UNREALLIBRARYEX_EDITOR_API FPropertyCustomizeHelper
{
	static void HeaderRowDrawProperty(class FDetailWidgetRow& HeaderRow, const TSharedPtr<IPropertyHandle>& PropertyHandle, bool bDisplayDefaultPropertyButtons = true);

	static uint32 GetNumChildren(const TSharedRef<IPropertyHandle>& PropertyHandle);

	static UObject* GetOuter(const TSharedRef<IPropertyHandle>& PropertyHandle);

	static bool IsInArray(const TSharedRef<IPropertyHandle>& PropertyHandle);

	static void StructBuilderDrawPropertys(class IDetailChildrenBuilder& StructBuilder, const TSharedRef<IPropertyHandle>& PropertyHandle, const TArray<FName>& ExcludePropertyNames = {}, UStruct* StopShowType = nullptr);

	static TSharedPtr<IPropertyHandle> GetPropertyHandleByName(TSharedRef<class IPropertyHandle> StructPropertyHandle, FName ChildPropertyName);

	template<typename ValueType>
	static ValueType GetValue(const TSharedPtr<IPropertyHandle>& PropertyHandle)
	{
		if (UObject* Outer = GetOuter(PropertyHandle.ToSharedRef()))
		{
			if (ValueType* Res = reinterpret_cast<ValueType*>(PropertyHandle->GetValueBaseAddress(reinterpret_cast<uint8*>(Outer))))
			{
				return *Res;
			}
		}
		return {};
	}

	//别在用户输入时的处理中用这个设置值，会导致PropertyHandle的Notify之类的处理无效
	template<typename ValueType>
	static ValueType* Value(const TSharedPtr<IPropertyHandle>& PropertyHandle)
	{
		return reinterpret_cast<ValueType*>(PropertyHandle->GetValueBaseAddress(reinterpret_cast<uint8*>(GetOuter(PropertyHandle.ToSharedRef()))));
	}

	static FPropertyAccess::Result SetValueFromFormattedString(const TSharedPtr<IPropertyHandle>& PropertyHandle, const FString& InVale, EPropertyValueSetFlags::Type Flags = EPropertyValueSetFlags::DefaultFlags);

	static void ForceSetValue(const TSharedPtr<IPropertyHandle>& PropertyHandle, const FText& Text);

	template<typename Type>
	static void SetValue(const TSharedPtr<IPropertyHandle>& PropertyHandle, const Type& Value, bool NotifyChange = true)
	{
		if (NotifyChange)
		{
			PropertyHandle->NotifyPreChange();
		}
		if (Type* Target = reinterpret_cast<Type*>(PropertyHandle->GetValueBaseAddress(reinterpret_cast<uint8*>(GetOuter(PropertyHandle.ToSharedRef())))))
		{
			*Target = Value;
			if (NotifyChange)
			{
				PropertyHandle->NotifyPostChange(EPropertyValueSetFlags::DefaultFlags);
			}
		}
	}

	static void SetObjectValue(const TSharedPtr<IPropertyHandle>& PropertyHandle, UObject* Object);
};

struct XD_UNREALLIBRARYEX_EDITOR_API FPropertyTypeWithInstancedButtonHelper
{
	static void CustomizeHeader(UClass* Type, const TSubclassOf<class UFactory>& Factory, const FText& ButtonName, TSharedRef<class IPropertyHandle> StructPropertyHandle, TSharedRef<class IPropertyHandle> InstancePropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils);
};
