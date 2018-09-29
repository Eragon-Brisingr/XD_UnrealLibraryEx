// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_AnimationFunctionLibrary.h"
#include <GameFramework/Character.h>
#include <Animation/AnimInstance.h>




float UXD_AnimationFunctionLibrary::GetMontageGlobalWeight(class ACharacter* Character, FName SlotName)
{
	if (Character)
	{
		return Character->GetMesh()->GetAnimInstance()->GetSlotMontageGlobalWeight(SlotName);
	}
	return 0.f;
}

bool UXD_AnimationFunctionLibrary::IsPlayingMontage(class ACharacter* Character, FName SlotName)
{
	return GetMontageGlobalWeight(Character, SlotName) > 0.f;
}
