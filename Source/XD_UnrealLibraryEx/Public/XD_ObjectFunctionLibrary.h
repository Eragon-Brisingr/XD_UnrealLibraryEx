// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XD_TemplateLibrary.h"
#include "XD_ObjectFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class XD_UNREALLIBRARYEX_API UXD_ObjectFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	public:
	//拷贝Object，保证新的Object一定不为RF_DefaultSubObject
	UFUNCTION(BlueprintPure, Category = "游戏|工具", meta = (DeterminesOutputType = "Object", DefaultToSelf = "Outer", HidePin = "Outer", CompactNodeTitle = "Copy"))
	static UObject* CopyObject(const UObject* Object, const UObject* Outer, const FName Name = NAME_None);

	template<typename Type>
	static Type* DuplicateObject(const Type* Object, const UObject* Outer, const FName Name = NAME_None)
	{
		return (Type*)CopyObject(Object, Outer, Name);
	}

	UFUNCTION(BlueprintPure, Category = "游戏|工具", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext, DeterminesOutputType = SoftActorPtr, BlueprintInternalUseOnly = "true"))
	static UObject* ResolveSoftObject(UObject* WorldContextObject, const TSoftObjectPtr<UObject>& SoftObjectPtr);

	//编辑器模式下SoftObjectPtr指向的Object和游戏中的Object不一样
	template<typename Type>
	static Type* ResolveSoftObject(UObject* WorldContextObject, const TSoftObjectPtr<Type>& SoftActorPtr)
	{
		return (Type*)ResolveSoftObject(WorldContextObject, SoftActorPtr, SoftActorPtr->GetClass());
	}

	UFUNCTION(BlueprintPure, Category = "游戏|工具")
	static FString GetObjectClassName(const UObject* Object);

	UFUNCTION(BlueprintPure, Category = "游戏|工具")
	static FString GetClassName(const UClass* Class);

	UFUNCTION(BlueprintPure, Category = "游戏|工具")
	static FString GetObjectPropertysDesc(const UObject* Object, UClass* StopAtClass, int32 LineKeepChars = 40);

	UFUNCTION(BlueprintCallable, Category = "游戏|工具", meta = (DeterminesOutputType = "Class"))
	static TArray<UClass*> GetAllSubclass(UClass* Class, bool ExculdeAbstract = true);

	template<typename Type>
	static TArray<TSubclassOf<Type>> GetAllSubclass(EClassFlags ExcludeFlags = EClassFlags::CLASS_Abstract)
	{
		return ArrayCast<TSubclassOf<Type>>(GetAllSubclassImpl(Type::StaticClass(), ExcludeFlags));
	}
private:
	static TArray<UClass*> GetAllSubclassImpl(UClass* Class, EClassFlags ExcludeFlags);
public:
	static bool CompareObject(const UObject* A, const UObject* B, UClass* StopAtClass);
};
