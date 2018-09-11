// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XD_EditorTickableActor.generated.h"

UCLASS(abstract)
class XD_UNREALLIBRARYEX_API AXD_EditorTickableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AXD_EditorTickableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool ShouldTickIfViewportsOnly() const override { return true; }
	virtual void TickActor(float DeltaSeconds, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
	
};

UCLASS(abstract)
class XD_UNREALLIBRARYEX_API AXD_OnlyEditorTickableActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AXD_OnlyEditorTickableActor();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool ShouldTickIfViewportsOnly() const override { return true; }
	virtual void TickActor(float DeltaSeconds, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
};

