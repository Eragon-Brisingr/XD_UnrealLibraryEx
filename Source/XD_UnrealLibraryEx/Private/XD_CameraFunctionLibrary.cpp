// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_CameraFunctionLibrary.h"
#if WITH_EDITOR
#include <LevelEditorViewport.h>
#include <Editor/EditorEngine.h>
#include <Editor.h>
#endif
#include <Kismet/GameplayStatics.h>
#include <Camera/PlayerCameraManager.h>




FVector UXD_CameraFunctionLibrary::GetCameraLocation(const UObject* WorldContextObject)
{
#if WITH_EDITOR
	if (!WorldContextObject->GetWorld()->IsGameWorld())
	{
		FLevelEditorViewportClient* client = static_cast<FLevelEditorViewportClient*>(GEditor->GetActiveViewport()->GetClient());
		return client->GetViewLocation();
	}
	else
#endif
	{
		if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(WorldContextObject, 0))
		{
			return CameraManager->GetCameraLocation();
		}
	}
	return FVector::ZeroVector;
}

FRotator UXD_CameraFunctionLibrary::GetCameraRotation(const UObject* WorldContextObject)
{
#if WITH_EDITOR
	if (!WorldContextObject->GetWorld()->IsGameWorld())
	{
		FLevelEditorViewportClient* client = static_cast<FLevelEditorViewportClient*>(GEditor->GetActiveViewport()->GetClient());
		return client->GetViewRotation();
	}
	else
#endif
	{
		if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(WorldContextObject, 0))
		{
			return CameraManager->GetCameraRotation();
		}
	}
	return FRotator::ZeroRotator;
}
