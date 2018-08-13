// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
	static UObject* CopyObject(const UObject* Object, UObject* Outer);

	template<typename Type>
	static Type* DuplicateObject(const Type* Object, UObject* Outer)
	{
		return (Type*)CopyObject(Object, Outer);
	}

	//编辑器模式下软引用引用到的对象可能和场景中的不同，所以编辑器下比较对象的名字确定是不是同一对象，而打包后直接比较对象地址
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (Object == SoftObjectReference)", CompactNodeTitle = "=="), Category = "游戏|工具")
	static bool EqualEqual_Object_SoftObject(UObject* A, const TAssetPtr<UObject>& B);

	//编辑器模式下软引用引用到的对象可能和场景中的不同，所以编辑器下比较对象的名字确定是不是同一对象，而打包后直接比较对象地址
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (SoftObjectReference == Object)", CompactNodeTitle = "=="), Category = "游戏|工具")
	static bool EqualEqual_SoftObject_Object(const TAssetPtr<UObject>& A, UObject* B);

	//编辑器模式下SoftObjectPtr指向的Actor和游戏中的Actor不一样，要通过比较名字来获取
	UFUNCTION(BlueprintPure, Category = "游戏|工具", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext, DeterminesOutputType = "TypeInfo", BlueprintInternalUseOnly = "true"))
	static AActor* GetActorSafe(UObject* WorldContextObject, const TSoftObjectPtr<AActor>& SoftActorPtr, TSubclassOf<AActor> Type);

	template<typename Type>
	static Type* GetSoftActorSafe(UObject* WorldContextObject, const TSoftObjectPtr<Type>& SoftActorPtr)
	{
		if (SoftActorPtr.IsValid())
		{
			return (Type*)GetActorSafe(WorldContextObject, SoftActorPtr, SoftActorPtr->GetClass());
		}
		return nullptr;
	}

	UFUNCTION(BlueprintPure, Category = "游戏|工具")
	static FString GetObjectClassName(const UObject* Object);

	UFUNCTION(BlueprintPure, Category = "游戏|工具")
	static FString GetClassName(const UClass* Class);

	UFUNCTION(BlueprintPure, Category = "游戏|工具")
	static FString GetObjectPropertysDesc(const UObject* Object, UClass* StopAtClass, int32 LineKeepChars = 40);

	UFUNCTION(BlueprintCallable, Category = "游戏|工具", meta = (DeterminesOutputType = "Class"))
	static TArray<UClass*> GetAllSubclass(UClass* Class, bool ContainsAbstract = false);

	template<typename Type>
	static TArray<TSubclassOf<Type>> GetAllSubclass(bool ContainsAbstract = false)
	{
		return ArrayCast<TSubclassOf<Type>>(GetAllSubclass(Type::StaticClass(), ContainsAbstract));
	}

	UFUNCTION(BlueprintPure, Category = "游戏|工具")
	static bool CompareObject(const UObject* A, const UObject* B, UClass* StopAtClass);
	
	
	
};
