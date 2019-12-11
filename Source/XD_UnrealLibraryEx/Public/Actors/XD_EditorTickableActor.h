// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <GameFramework/Actor.h>
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

#if WITH_EDITOR
	virtual bool ShouldTickIfViewportsOnly() const override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	virtual void TickActor(float DeltaSeconds, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "控制", meta = (DisplayName = "刷新"))
	uint8 Fresh : 1;

	UPROPERTY(EditAnywhere, Category = "控制", meta = (DisplayName = "暂停"))
	uint8 Pause : 1;
#endif

	UFUNCTION(BlueprintNativeEvent, Category = "控制")
	void WhenFreshClicked();
	void WhenFreshClicked_Implementation() {}
};

UCLASS(abstract)
class XD_UNREALLIBRARYEX_API AXD_OnlyEditorTickableActor : public AXD_EditorTickableActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AXD_OnlyEditorTickableActor();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual bool ShouldTickIfViewportsOnly() const override;
#endif
};

