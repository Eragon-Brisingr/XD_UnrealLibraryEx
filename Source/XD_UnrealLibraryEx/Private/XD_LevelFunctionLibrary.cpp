// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_LevelFunctionLibrary.h"
#include <Engine/LevelScriptActor.h>
#include <Engine/LevelStreaming.h>

FIntVector UXD_LevelFunctionLibrary::GetWorldOffset(const UObject* WorldContextObject, const FIntVector& OldWorldOrigin)
{
	return OldWorldOrigin - UGameplayStatics::GetWorldOriginLocation(WorldContextObject);
}

FVector UXD_LevelFunctionLibrary::GetFixedWorldLocation(const UObject* WorldContextObject, const FIntVector& OldWorldOrigin, const FVector& WorldLocation)
{
	FIntVector WorldOffset = UXD_LevelFunctionLibrary::GetWorldOffset(WorldContextObject, OldWorldOrigin);
	return { WorldLocation.X + WorldOffset.X, WorldLocation.Y + WorldOffset.Y, WorldLocation.Z + WorldOffset.Z };
}

ULevelStreaming* UXD_LevelFunctionLibrary::GetStreamLevel(ALevelScriptActor* LevelScriptActor)
{
	for (auto It = LevelScriptActor->GetWorld()->GetStreamingLevels().CreateConstIterator(); It; ++It)
	{
		if ((*It)->GetWorldAssetPackageFName() == LevelScriptActor->GetLevel()->GetOutermost()->GetFName())
		{
			return *It;
		}
	}
	return nullptr;
}

ULevel* UXD_LevelFunctionLibrary::GetLevel(AActor* Actor)
{
	return Actor->GetLevel();
}

UWorld* UXD_LevelFunctionLibrary::GetCurrentLevelWorld(ULevel* Level)
{
	return Cast<UWorld>(Level->GetOuter());
}

class AWorldSettings* UXD_LevelFunctionLibrary::GetCurrentLevelWorldSettings(class ULevel* Level)
{
	return GetCurrentLevelWorld(Level)->GetWorldSettings();
}

TArray<AActor*>& UXD_LevelFunctionLibrary::GetLevelActors(ULevel* Level)
{
	return Level->Actors;
}

ALevelBounds* UXD_LevelFunctionLibrary::GetLevelBounds(ULevel* Level)
{
	return Level->LevelBoundsActor.Get();
}

TArray<ULevelStreaming*> UXD_LevelFunctionLibrary::GetWorldStreamLevel(UObject* WorldContextObject)
{
	return WorldContextObject->GetWorld()->GetStreamingLevels();
}

const TArray<ULevel*>& UXD_LevelFunctionLibrary::GetAllLevel(UObject* WorldContextObject)
{
	return WorldContextObject->GetWorld()->GetLevels();
}

FString UXD_LevelFunctionLibrary::GetLevelName(ULevel* Level)
{
	FString LevelFullName = Level->GetOutermost()->GetName();
	int32 Index;
	if (LevelFullName.FindLastChar(TEXT('/'), Index))
	{
		return LevelFullName.Right(LevelFullName.Len() - Index - 1);
	}
	return LevelFullName;
}

FString UXD_LevelFunctionLibrary::GetLevelFullName(ULevel* Level)
{
	return Level->GetOutermost()->GetName();
}

ALevelScriptActor* UXD_LevelFunctionLibrary::GetLevelScriptActor(ULevel* Level)
{
	return Level ? Level->LevelScriptActor : nullptr;
}

class ULevel* UXD_LevelFunctionLibrary::GetLoadedLevel(ULevelStreaming* LevelStreaming)
{
	return LevelStreaming->GetLoadedLevel();
}

ULevelStreamingHelper* ULevelStreamingHelper::WhenLevelLoaded(ULevelStreaming* LevelStreaming)
{
	if (LevelStreaming)
	{
		ULevelStreamingHelper* LevelStreamingHelper = NewObject<ULevelStreamingHelper>(LevelStreaming);
		//加强引用
		LevelStreamingHelper->SetFlags(RF_StrongRefOnFrame);
		LevelStreamingHelper->_LevelStreaming = LevelStreaming;
		LevelStreaming->OnLevelLoaded.AddDynamic(LevelStreamingHelper, &ULevelStreamingHelper::_WhenLevelLoaded);
		return LevelStreamingHelper;
	}
	return nullptr;
}

void ULevelStreamingHelper::_WhenLevelLoaded()
{
	if (_LevelStreaming.IsValid())
	{
		_LevelStreaming->GetWorld()->GetTimerManager().SetTimerForNextTick([this]
		{
			OnLevelLoaded.Broadcast(_LevelStreaming->GetLoadedLevel());
		});
	}
	SetReadyToDestroy();
}



