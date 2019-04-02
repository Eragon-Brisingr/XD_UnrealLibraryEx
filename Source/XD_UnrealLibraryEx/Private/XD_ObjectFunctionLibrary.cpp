// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_ObjectFunctionLibrary.h"
#include <GameFramework/Actor.h>
#include <EngineUtils.h>
#include <AssetRegistryModule.h>
#include "XD_GameTypeEx.h"

UObject* UXD_ObjectFunctionLibrary::CopyObject(const UObject* Object, UObject* Outer, const FName Name /*= NAME_None*/)
{
	if (Object)
	{
		UObject* Res = ::DuplicateObject(Object, Outer, Name);
		Res->ClearFlags(RF_InPackageFlags);
		return Res;
	}
	return nullptr;
}


bool UXD_ObjectFunctionLibrary::EqualEqual_Object_SoftObject(UObject* A, const TAssetPtr<UObject>& B)
{
#if WITH_EDITOR
	if (A && B.Get())
	{
		return A->GetName() == B->GetName();
	}
	else
	{
		return A == B.Get();
	}
#else
	return A == B.Get();
#endif
}

bool UXD_ObjectFunctionLibrary::EqualEqual_SoftObject_Object(const TAssetPtr<UObject>& A, UObject* B)
{
	return EqualEqual_Object_SoftObject(B, A);
}

AActor* UXD_ObjectFunctionLibrary::GetActorSafe(UObject* WorldContextObject, const TSoftObjectPtr<AActor>& SoftActorPtr, TSubclassOf<AActor> Type)
{
#if WITH_EDITOR
	if (WorldContextObject || SoftActorPtr.Get())
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			for (TActorIterator<AActor> It(World, Type ? Type : AActor::StaticClass()); It; ++It)
			{
				AActor* Actor = *It;
				if (Actor->GetName() == SoftActorPtr->GetName())
				{
					return Actor;
				}
			}
		}
	}
	return nullptr;
#else
	return SoftActorPtr.Get();
#endif
}

FString UXD_ObjectFunctionLibrary::GetObjectClassName(const UObject* Object)
{
	if (Object)
	{
		return GetClassName(Object->GetClass());
	}
	else
	{
		return TEXT("None");
	}
}

FString UXD_ObjectFunctionLibrary::GetClassName(const UClass* Class)
{
	if (Class)
	{
#if WITH_EDITOR
		FString ClassName = Class->GetDisplayNameText().ToString();
#else
		FString ClassName = Class->GetName();
		ClassName.RemoveFromEnd(TEXT("_C"));
#endif
		return ClassName;
	}
	else
	{
		return TEXT("None");
	}
}

FString UXD_ObjectFunctionLibrary::GetObjectPropertysDesc(const UObject* Object, UClass* StopAtClass, int32 LineKeepChars /*= 40*/)
{
	if (!Object)
	{
		return TEXT("None");
	}

	FString ReturnDesc;
	FString PropertyDesc;
	for (UProperty* TestProperty = Object->GetClass()->PropertyLink; TestProperty; TestProperty = TestProperty->PropertyLinkNext)
	{
		if (TestProperty->GetOuter() == StopAtClass)
		{
			break;
		}

		// 必须是可编辑的变量才显示
		if (!TestProperty->HasAnyPropertyFlags(CPF_Edit) || TestProperty->HasAnyPropertyFlags(CPF_Transient | CPF_EditConst | CPF_DisableEditOnInstance | CPF_DisableEditOnTemplate))
		{
			continue;
		}

		// 变量名与值
		{
			const uint8* PropertyAddr = TestProperty->ContainerPtrToValuePtr<uint8>(Object);

			FString ExportedStringValue;

			if (const UFloatProperty* FloatProp = Cast<const UFloatProperty>(TestProperty))
			{
				const float FloatValue = FloatProp->GetPropertyValue(PropertyAddr);
				ExportedStringValue = FString::SanitizeFloat(FloatValue);
			}
			//软引用只用引用的对象名
			else if (const USoftObjectProperty* SoftObjectProp = Cast<const USoftObjectProperty>(TestProperty))
			{
				FSoftObjectPtr SoftObjectPtr = SoftObjectProp->GetPropertyValue(PropertyAddr);
				if (!SoftObjectPtr.IsNull())
				{
					SoftObjectPtr.ToSoftObjectPath().GetSubPathString().Split(TEXT("."), nullptr, &ExportedStringValue, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
				}
				else
				{
					ExportedStringValue = TEXT("None");
				}
			}
			else if (const UClassProperty* ClassProperty = Cast<const UClassProperty>(TestProperty))
			{
				UClass* Class = Cast<UClass>(ClassProperty->GetPropertyValue(PropertyAddr));
				ExportedStringValue = GetClassName(Class);
			}
			else
			{
				TestProperty->ExportTextItem(ExportedStringValue, PropertyAddr, NULL, NULL, PPF_PropertyWindow, NULL);
			}

			const bool bIsBool = TestProperty->IsA(UBoolProperty::StaticClass());
#if WITH_EDITOR
			FString PropertyName = TestProperty->GetDisplayNameText().ToString();
#else
			FString PropertyName = TestProperty->GetNameCPP();
#endif
			if (ExportedStringValue.Len() > LineKeepChars)
			{
				ExportedStringValue.RemoveAt(LineKeepChars, ExportedStringValue.Len() - LineKeepChars);
				ExportedStringValue.Append(TEXT("..."));
			}

			PropertyDesc += FString::Printf(TEXT("%s: %s\n"), *FName::NameToDisplayString(PropertyName, bIsBool), *ExportedStringValue);

		}
	}

	if (PropertyDesc.Len() > 0)
	{
		PropertyDesc.RemoveFromEnd(TEXT("\n"));
		ReturnDesc += PropertyDesc;
	}

	return ReturnDesc;
}


TArray<UClass*> UXD_ObjectFunctionLibrary::GetAllSubclass(UClass* Class, bool ContainsAbstract)
{
	TArray<UClass*> Res;

	TArray<UClass*> DerivedClasses;
	GetDerivedClasses(Class, DerivedClasses, true);
	for (UClass* DerivedClasse : DerivedClasses)
	{
		if (ContainsAbstract && DerivedClasse->HasAnyClassFlags(CLASS_Abstract))
		{
			continue;
		}

		if (DerivedClasse->GetName().StartsWith("REINST") || DerivedClasse->GetName().StartsWith("SKEL"))
		{
			continue;
		}

		Res.Add(DerivedClasse);
	}

  	static TArray<UClass*> SeachedClasses;
	if (!SeachedClasses.Contains(Class))
	{
  		SeachedClasses.Add(Class);

		TArray<UClass*>& NativeParentClassList = DerivedClasses;
		NativeParentClassList.Add(Class);

  		//搜集蓝图类型
  		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
  		TArray<FAssetData> BlueprintList;
  
  		FARFilter Filter;
  		Filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
  		AssetRegistryModule.Get().GetAssets(Filter, BlueprintList);
  
  		TMap<FString, FAssetData> ParentInfoMaps;
  
  		for (FAssetData& AssetData : BlueprintList)
  		{
  			FString AssetClassName;
  			FString AssetParentClassName;
  			if (AssetData.GetTagValue("GeneratedClass", AssetClassName) && AssetData.GetTagValue("ParentClass", AssetParentClassName))
  			{
  				UObject* Outer1(nullptr);
  				ResolveName(Outer1, AssetClassName, false, false);
  
  				UObject* Outer2(nullptr);
  				ResolveName(Outer2, AssetParentClassName, false, false);
  
  				ParentInfoMaps.Add(AssetClassName) = AssetData;
  			}
  		}
  
  		struct FCheckParentHelper
  		{
  			FCheckParentHelper(const TMap<FString, FAssetData>& ParentInfoMaps, const TArray<UClass*>& NativeParentClassList)
  				:ParentInfoMaps(ParentInfoMaps), NativeParentClassList(NativeParentClassList)
  			{}
  			const TMap<FString, FAssetData>& ParentInfoMaps;
  			const TArray<UClass*>& NativeParentClassList;
  
  			bool IsParent(const FString& Name)
  			{
  				FString ParentName;
  				ParentInfoMaps[Name].GetTagValue("ParentClass", ParentName);
  
  				UObject* Outer2(nullptr);
  				ResolveName(Outer2, ParentName, false, false);
  
  				if (NativeParentClassList.ContainsByPredicate([&](const UClass* Class) {return Class->GetName() == ParentName; }))
  				{
  					return true;
  				}
  				else if (ParentInfoMaps.Contains(ParentName))
  				{
  					return IsParent(ParentName);
  				}
  				return false;
  			}
  		}CheckParentHelper(ParentInfoMaps, NativeParentClassList);
  
  		for (TPair<FString, FAssetData>& NameAndData : ParentInfoMaps)
  		{
  			if (CheckParentHelper.IsParent(NameAndData.Key))
  			{
  				FString PathName = NameAndData.Value.PackageName.ToString();
  				if (UBlueprint* LoadedBlueprint = ConstructorHelpersInternal::FindOrLoadObject<UBlueprint>(PathName))
  				{
					UClass* DerivedClass = LoadedBlueprint->GeneratedClass;
					if (ContainsAbstract && DerivedClass->HasAnyClassFlags(CLASS_Abstract))
					{
						continue;
					}
					if (Res.Contains(DerivedClass))
					{
						continue;
					}
  					Res.Add(DerivedClass);
  				}
  			}
  		}
  	}

	return Res;
}

bool UXD_ObjectFunctionLibrary::CompareObject(const UObject* A, const UObject* B, UClass* StopAtClass)
{
	struct CompareObjectHelper
	{
		static bool ShouldCompareProperty(const UProperty* Property)
		{
			// Ignore components & transient properties
			const bool bIsTransient = !!(Property->PropertyFlags & CPF_Transient);
			// 			const bool bIsComponent = !!(Property->PropertyFlags & (CPF_InstancedReference | CPF_ContainsInstancedReference));
			// 			const bool bShouldCompare = !(bIsTransient || bIsComponent);

			//			return bShouldCompare;
			return !bIsTransient;
		}
	};

	if (A && B && A->GetClass() == B->GetClass())
	{
		if (A == B)
			return true;

		for (TFieldIterator<UProperty> PropIt(A->GetClass(), EFieldIteratorFlags::IncludeSuper); PropIt; ++PropIt)
		{
			if (PropIt->GetOuter() == StopAtClass->GetSuperClass())
			{
				break;
			}

			UProperty* Prop = *PropIt;

			if (CompareObjectHelper::ShouldCompareProperty(Prop))
			{
				if (UObjectProperty* ObjectProperty = Cast<UObjectProperty>(Prop))
				{
					const UObject* A_ContainObject = ObjectProperty->GetObjectPropertyValue(ObjectProperty->ContainerPtrToValuePtr<void>(A));
					const UObject* B_ContainObject = ObjectProperty->GetObjectPropertyValue(ObjectProperty->ContainerPtrToValuePtr<void>(B));
					if (A_ContainObject && B_ContainObject)
					{
						if (A_ContainObject->GetOuter() == A && B_ContainObject->GetOuter() == B)
						{
							UXD_ObjectFunctionLibrary::CompareObject(A_ContainObject, B_ContainObject, nullptr);
						}
						else if (A_ContainObject != B_ContainObject)
						{
							return false;
						}
					}
					else if (A_ContainObject || B_ContainObject)
					{
						return false;
					}
				}
				else
				{
					for (int32 Index = 0; Index < Prop->ArrayDim; Index++)
					{
						FString A_PropertyText;
						Prop->ExportText_InContainer(Index, A_PropertyText, A, A, const_cast<UObject*>(A), PPF_SimpleObjectText);
						FString B_PropertyText;
						Prop->ExportText_InContainer(Index, B_PropertyText, B, B, const_cast<UObject*>(B), PPF_SimpleObjectText);
						if (A_PropertyText != B_PropertyText)
						{
							return false;
						}
					}
				}
			}
		}
		return true;
	}
	return false;
}



