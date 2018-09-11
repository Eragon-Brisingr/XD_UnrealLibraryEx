// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_EditorTickableActor.h"


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
	if (GetWorld()->WorldType == EWorldType::Editor)
	{
		Super::Tick(DeltaTime);
	}
}

void AXD_OnlyEditorTickableActor::TickActor(float DeltaSeconds, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	FEditorScriptExecutionGuard ScriptGuard;
	Super::TickActor(DeltaSeconds, TickType, ThisTickFunction);
}
