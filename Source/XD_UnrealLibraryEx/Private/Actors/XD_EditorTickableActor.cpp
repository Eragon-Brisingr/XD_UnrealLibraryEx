// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/XD_EditorTickableActor.h"


// Sets default values
AXD_EditorTickableActor::AXD_EditorTickableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AXD_EditorTickableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AXD_EditorTickableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#if WITH_EDITOR
bool AXD_EditorTickableActor::ShouldTickIfViewportsOnly() const
{
	if (UWorld* World = GetWorld())
	{
		return World->WorldType != EWorldType::EditorPreview && !Pause;
	}
	return false;
}

void AXD_EditorTickableActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AXD_EditorTickableActor, Fresh))
	{
		Fresh = false;
		FEditorScriptExecutionGuard ScriptGuard;
		Tick(10000000.f);
	}
}

#endif

void AXD_EditorTickableActor::TickActor(float DeltaSeconds, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	FEditorScriptExecutionGuard ScriptGuard;
	Super::TickActor(DeltaSeconds, TickType, ThisTickFunction);
}

AXD_OnlyEditorTickableActor::AXD_OnlyEditorTickableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AXD_OnlyEditorTickableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR
bool AXD_OnlyEditorTickableActor::ShouldTickIfViewportsOnly() const
{
	if (UWorld* World = GetWorld())
	{
		return World->WorldType == EWorldType::Editor && !Pause;
	}
	return false;
}
#endif
