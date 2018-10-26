// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_PropertyCustomizationEx.h"
#include <DetailWidgetRow.h>
#include <IDetailChildrenBuilder.h>

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
	return Outers[0];
}

bool FPropertyCustomizeHelper::IsInArray(const TSharedRef<IPropertyHandle>& PropertyHandle)
{
	return PropertyHandle->GetIndexInArray() != INDEX_NONE;
}

void FPropertyCustomizeHelper::StructBuilderDrawPropertys(class IDetailChildrenBuilder& StructBuilder, const TSharedRef<IPropertyHandle>& PropertyHandle, const TArray<FString>& ExcludePropertyNames)
{
	for (uint32 ChildIndex = 0; ChildIndex < GetNumChildren(PropertyHandle); ++ChildIndex)
	{
		const TSharedRef< IPropertyHandle > ChildHandle = PropertyHandle->GetChildHandle(ChildIndex).ToSharedRef();

		if (!ExcludePropertyNames.Contains(ChildHandle->GetProperty()->GetName()))
		{
			StructBuilder.AddProperty(ChildHandle);
		}
	}
}

TSharedPtr<IPropertyHandle> FPropertyCustomizeHelper::GetPropertyHandleByName(TSharedRef<class IPropertyHandle> StructPropertyHandle, FString ChildPropertyName)
{
	uint32 NumChildren;
	StructPropertyHandle->GetNumChildren(NumChildren);
	for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex)
	{
		const TSharedRef< IPropertyHandle > ChildHandle = StructPropertyHandle->GetChildHandle(ChildIndex).ToSharedRef();
		if (ChildHandle->GetProperty()->GetName() == ChildPropertyName)
		{
			return ChildHandle;
		}
	}
	return nullptr;
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
	FTextStringHelper::WriteToString(StringValue, Text);
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

void IPropertyTypeWithInstancedButtonHelper::CustomizeHeader(UClass* Type, const FText& ButtonName, TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
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
			StructPropertyHandle->CreatePropertyValueWidget()
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Right)
		[
			SNew(SButton)
			.OnClicked_Lambda([=]() {
		FPropertyCustomizeHelper::SetObjectValue(StructPropertyHandle, NewObject<UObject>(FPropertyCustomizeHelper::GetOuter(StructPropertyHandle), Type));
		return FReply::Handled();
	})
		.IsEnabled_Lambda([=]() {
		if (UObject* Outer = FPropertyCustomizeHelper::GetOuter(StructPropertyHandle))
		{
			if (Outer->HasAnyFlags(RF_ClassDefaultObject))
			{
				return false;
			}
		}
		if (UObject* Instance = FPropertyCustomizeHelper::GetValue<UObject*>(StructPropertyHandle))
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
		];
}
