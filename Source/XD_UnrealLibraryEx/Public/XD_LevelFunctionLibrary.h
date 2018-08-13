// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <Kismet/BlueprintAsyncActionBase.h>
#include "XD_LevelFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class XD_UNREALLIBRARYEX_API UXD_LevelFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//关于关卡
public:
	UFUNCTION(BlueprintPure, Category = "游戏|读档", meta = (WorldContext = "WorldContextObject"))
	static FIntVector GetWorldOffset(const UObject* WorldContextObject, const FIntVector& OldWorldOrigin);

	UFUNCTION(BlueprintPure, Category = "游戏|读档", meta = (WorldContext = "WorldContextObject"))
	static FVector GetFixedWorldLocation(const UObject* WorldContextObject, const FIntVector& OldWorldOrigin, const FVector& WorldLocation);

	UFUNCTION(BlueprintPure, Category = "游戏|关卡")
	static class ULevelStreaming* GetStreamLevel(class ALevelScriptActor* LevelScriptActor);

	UFUNCTION(BlueprintPure, Category = "游戏|关卡")
	static ULevel* GetLevel(AActor* Actor);

	//获得当前关卡的世界，可能为非主世界
	UFUNCTION(BlueprintPure, Category = "游戏|关卡")
	static UWorld* GetCurrentLevelWorld(ULevel* Level);

	//获得当前关卡的世界设置，可能为非主世界设置
	UFUNCTION(BlueprintPure, Category = "游戏|关卡")
	static class AWorldSettings* GetCurrentLevelWorldSettings(class ULevel* Level);

	UFUNCTION(BlueprintCallable, Category = "游戏|关卡")
	static TArray<AActor*>& GetLevelActors(ULevel* Level);

	UFUNCTION(BlueprintPure, Category = "游戏|关卡")
	static class ALevelBounds* GetLevelBounds(ULevel* Level);

	UFUNCTION(BlueprintCallable, Category = "游戏|关卡", meta = (WorldContext = "WorldContextObject"))
	static TArray<class ULevelStreaming*> GetWorldStreamLevel(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "游戏|关卡", meta = (WorldContext = "WorldContextObject"))
	static const TArray<class ULevel*>& GetAllLevel(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "游戏|关卡")
	static FString GetLevelName(ULevel* Level);

	UFUNCTION(BlueprintPure, Category = "游戏|关卡")
	static FString GetLevelFullName(ULevel* Level);

	UFUNCTION(BlueprintPure, Category = "游戏|关卡")
	static ALevelScriptActor* GetLevelScriptActor(ULevel* Level);

	UFUNCTION(BlueprintPure, Category = "游戏|关卡")
	static class ULevel* GetLoadedLevel(class ULevelStreaming* LevelStreaming);


};

UCLASS()
class ULevelStreamingHelper : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "环境|交互", meta = (BlueprintInternalUseOnly = "true"))
	static ULevelStreamingHelper* WhenLevelLoaded(class ULevelStreaming* LevelStreaming);
private:
	
	TWeakObjectPtr<class ULevelStreaming> _LevelStreaming;

	UFUNCTION()
	void _WhenLevelLoaded();
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelLoaded, ULevel*, Level);
	UPROPERTY(BlueprintAssignable)
	FOnLevelLoaded OnLevelLoaded;
};
