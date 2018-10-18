// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XD_GameTypeEx.h"
#include "XD_BlueprintFunctionLibrary.generated.h"

/**
* 这边的函数一般为暴露给蓝图用的包装函数，C++中不需要使用
*/
UENUM(BlueprintType)
enum class EEditorWorldType : uint8
{
	/** An untyped world, in most cases this will be the vestigial worlds of streamed in sub-levels */
	None,

	/** The game world */
	Game,

	/** A world being edited in the editor */
	Editor,

	/** A Play In Editor world */
	PIE,

	/** A preview world for an editor tool */
	EditorPreview,

	/** A preview world for a game */
	GamePreview,

	/** An editor world that was loaded but not currently being edited in the level editor */
	Inactive
};

UCLASS()
class XD_UNREALLIBRARYEX_API UXD_BlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "游戏|工具")
	static bool IsValid(const FBPDelegateHandle& Handle);

	//Tick
public:
	DECLARE_DYNAMIC_DELEGATE_OneParam(FBPTickerDelegate, float, DeltaSeconds);

	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBPTickerDelegateWithReturn, float, DeltaSeconds, bool&, IsContinueTick);

	UFUNCTION(BlueprintCallable, Category = "游戏|工具")
	static FBPDelegateHandle AddTicker(const FBPTickerDelegate& BPTickerDelegate, float InDelay = 0.f);

	UFUNCTION(BlueprintCallable, Category = "游戏|工具")
	static void RemoveTicker(const FBPDelegateHandle& BPDelegateHandle);

	static bool TickerTick(float DeltaSeconds, FBPTickerDelegate BPTickerDelegate);

	UFUNCTION(BlueprintCallable, Category = "游戏|工具")
	static FBPDelegateHandle AddTickerWithReturn(const FBPTickerDelegateWithReturn& BPTickerDelegate, float InDelay = 0.f);

	static bool TickerTick(float DeltaSeconds, FBPTickerDelegateWithReturn BPTickerDelegate);

	//
public:
	DECLARE_DYNAMIC_DELEGATE_OneParam(FBPActorSpawnedinWorldDelegate, class AActor*, Actor);

	//Actor在游戏中生成就会调用
	UFUNCTION(BlueprintCallable, Category = "游戏|工具", meta = (WorldContext = "WorldContextObject"))
	static FBPDelegateHandle AddOnActorSpawnedInWorldEvent(const UObject* WorldContextObject, const FBPActorSpawnedinWorldDelegate& OnActorSpawnedEvent);

	UFUNCTION(BlueprintCallable, Category = "游戏|工具", meta = (WorldContext = "WorldContextObject"))
	static void RemoveOnActorSpawnedInWorldEvent(const UObject* WorldContextObject, const FBPDelegateHandle& DelegateHandle);

	//动画
public:
	UFUNCTION(BlueprintPure, Category = "游戏|动画")
	static float GetBlendInTime(UAnimMontage* Montage);

	UFUNCTION(BlueprintPure, Category = "游戏|动画")
	static float GetBlendOutTime(UAnimMontage* Montage);

	UFUNCTION(BlueprintPure, Category = "游戏|动画")
	static const TArray<class UAnimMetaData*>& GetMetaData(const UAnimationAsset* AnimationAsset);

	UFUNCTION(BlueprintPure, Category = "游戏|动画")
	static bool IsContainMetaDataType(const UAnimationAsset* AnimationAsset, TSubclassOf<class UAnimMetaData> AnimMetaDataType);
	
	//数组用
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FFindElementFunction, UObject*, Element, bool&, Result);
	UFUNCTION(BlueprintPure, Category = "游戏|工具|数组")
	static UObject* FindItem(const TArray<UObject*>& Array, const FFindElementFunction& FindElementFunction);

	DECLARE_DYNAMIC_DELEGATE_ThreeParams(FSortFunction, UObject*, A, UObject*, B, bool&, Result);
	UFUNCTION(BlueprintCallable, Category = "游戏|工具|数组")
	static void Sort(UPARAM(Ref)TArray<UObject*>& Array, const FSortFunction& SortFunction);

	DECLARE_DYNAMIC_DELEGATE_FourParams(FSortCaptureOneParameterFunction, UObject*, A, UObject*, B, UObject*, P1, bool&, Result);
	UFUNCTION(BlueprintCallable, Category = "游戏|工具|数组")
	static void SortCaptureOneParameter(UPARAM(Ref)TArray<UObject*>& Array, UObject* CaptureParameter, const FSortCaptureOneParameterFunction& SortFunction);

	DECLARE_DYNAMIC_DELEGATE_TwoParams(FFindFunction, UObject*, A, bool&, Result);
	UFUNCTION(BlueprintPure, Category = "游戏|工具|数组")
	static int32 Find(UPARAM(Ref)TArray<UObject*>& Array, const FFindFunction& FindFunction);

	static void Generic_SortStructArray_UserDefined(void* TargetArray, const UArrayProperty* ArrayProp, UObject* OwnerObject, FName FunctionName);
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (DisplayName = "SortArray_ByFunctionName", DefaultToSelf = "FunctionFromObject", ArrayParm = "CustomStruct"), Category = "工具|数组")
	static void SortStructArray_UserDefined(const TArray<int32>& CustomStruct, UObject* FunctionFromObject, FName FunctionName);
	DECLARE_FUNCTION(execSortStructArray_UserDefined)
	{
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<UArrayProperty>(NULL);
		void* ArrayAddr = Stack.MostRecentPropertyAddress;
		UArrayProperty* ArrayProperty = Cast<UArrayProperty>(Stack.MostRecentProperty);
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}
		P_GET_OBJECT(UObject, FunctionFromObject);
		P_GET_PROPERTY(UNameProperty, FunctionName);
		P_FINISH;
		P_NATIVE_BEGIN;
		Generic_SortStructArray_UserDefined(ArrayAddr, ArrayProperty, FunctionFromObject, FunctionName);
		P_NATIVE_END;
	};

	//FString
public:
	UFUNCTION(BlueprintPure, meta=(DisplayName = "String < String", CompactNodeTitle = "<", Keywords = "< less"), Category="Math|String")
	static bool Less_StringString(const FString& A, const FString& B);

	UFUNCTION(BlueprintPure, meta=(DisplayName = "String > String", CompactNodeTitle = ">", Keywords = "> greater"), Category="Math|String")
	static bool Greater_StringString(const FString& A, const FString& B);
	
public:
	//默认蓝图版本很多组件只允许它的Owner销毁，此版本不进行该检查
	UFUNCTION(BlueprintCallable, Category = "游戏|工具")
	static void DestroyComponent(UActorComponent* Component);

	UFUNCTION(BlueprintCallable, Category = "游戏|工具")
	static void SetLightingChannels(UPrimitiveComponent* PrimitiveComponent, bool Channel_0, bool Channel_1, bool Channel_2);

	//扩展UPathFollowingComponent功能
public:
	UFUNCTION(BlueprintCallable, Category = "游戏|AI")
	static void PauseMove(class UPathFollowingComponent* PathFollowingComponent);

	UFUNCTION(BlueprintCallable, Category = "游戏|AI")
	static void ResumeMove(class UPathFollowingComponent* PathFollowingComponent);

	UFUNCTION(BlueprintPure, Category = "游戏|寻路")
	static void BreakNavigationLink(const FNavigationLink& NavigationLink, FVector& Left, FVector& Right);

	UFUNCTION(BlueprintPure, Category = "游戏|AI", meta = (WorldContext = "WorldContextObject"))
	static bool IsAreaExistNavigationMesh(UObject* WorldContextObject, FVector TestLoc);

	UFUNCTION(BlueprintPure, Category = "游戏|工具")
	static FVector GetCurrentTargetLocation(UPathFollowingComponent* PathFollowingComponent);
	
	//Actor
public:
	UFUNCTION(BlueprintCallable, Category = "游戏|工具", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext, DeterminesOutputType = "ActorClass"))
	static AActor* SpawnActorFromClass(UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, const FTransform& Transform, AActor* Owner, APawn* Instigator, ESpawnActorCollisionHandlingMethod CollisionHandling = ESpawnActorCollisionHandlingMethod::Undefined);

	UFUNCTION(BlueprintCallable, Category = "游戏|工具", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext, DeterminesOutputType = "ActorClass"))
	static AActor* SpawnActorInLevel(UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, const FTransform& Transform, AActor* InLevelActor, APawn* Instigator);
	
	//回放系统
public:
	UFUNCTION(BlueprintCallable, Category = "回放系统", meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "AdditionalOptions"))
	static void StartRecordingReplay(const FString& InName, const FString& FriendlyName, const TArray<FString>& AdditionalOptions, UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "回放系统", meta = (WorldContext = "WorldContextObject"))
	static void StopRecordingReplay(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "回放系统", meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "AdditionalOptions"))
	static void PlayReplay(const FString& InName, UWorld* WorldOverride, const TArray<FString>& AdditionalOptions, UObject* WorldContextObject);
	
	//Object
public:
	UFUNCTION(BlueprintPure, Category = "游戏|工具", meta = (DefaultToSelf = "Object"))
	static UWorld* GetWorldFromObject(UObject* Object);
	
	UFUNCTION(BlueprintPure, Category = "游戏|工具", meta = (DefaultToSelf = "Object", HidePin = "Object"))
	static UObject* GetOuter(UObject* Object);

	UFUNCTION(BlueprintPure, Category = "游戏|工具", meta = (DeterminesOutputType = "Class"))
	static UObject* GetClassDefaultObject(TSubclassOf<UObject> Class);

	//Timer
public:
	UFUNCTION(BlueprintCallable, Category="游戏|工具")
	static FTimerHandle SetTimerByEvent_WithoutFindHandle(FTimerDynamicDelegate Event, float Time, bool bLooping);

	//FRuntimeFloatCurve
public:
	UFUNCTION(BlueprintPure, Category = "游戏|工具")
	static float GetRuntimeFloatCurveValue(const FRuntimeFloatCurve& RuntimeFloatCurve, float Time, float DefaultValue = 0.f);

	//Nav
public:
	UFUNCTION(BlueprintCallable, Category = "游戏|工具")
	static void SetNavigationLinkParameters(UPARAM(Ref)FNavigationLink& NavigationLink, FVector Left, FVector Right, bool UseSnapHeight = false, float SnapHeight = 50.f);

	//编辑器下功能
public:
	UFUNCTION(BlueprintPure, Category = "游戏|工具", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext = "true", DevelopmentOnly))
	static bool IsPlayInEditorPreview(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "游戏|工具", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext = "true", DevelopmentOnly))
	static bool IsPlayInGame(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "游戏|工具", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext = "true", DevelopmentOnly))
	static bool IsPlayInPIE(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "游戏|工具", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext = "true", DevelopmentOnly))
	static EEditorWorldType GetWorldType(UObject* WorldContextObject);
	
};
