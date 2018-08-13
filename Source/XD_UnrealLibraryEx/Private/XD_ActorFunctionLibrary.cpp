// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_ActorFunctionLibrary.h"

UActorComponent* UXD_ActorFunctionLibrary::AddComponent(AActor* Actor, TSubclassOf<class UActorComponent> Class, FName Name /*= NAME_None*/)
{
	if (Actor)
	{
		UActorComponent* Component = NewObject<UActorComponent>(Actor, Class, Name);
		Actor->AddOwnedComponent(Component);
		Component->RegisterComponent();
		return Component;
	}
	return nullptr;
}
