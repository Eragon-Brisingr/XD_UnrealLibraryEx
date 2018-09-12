// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_MathFunctionLibrary.h"




float UXD_MathFunctionLibrary::FInterpTo_Advance(float Current, float Target, float DeltaTime, float IncreaseSpeed, float DecreaseSpeed)
{
	return FMath::FInterpTo(Current, Target, DeltaTime, Current < Target ? IncreaseSpeed : DecreaseSpeed);
}

float UXD_MathFunctionLibrary::FInterpConstantTo_Advance(float Current, float Target, float DeltaTime, float IncreaseSpeed, float DecreaseSpeed)
{
	return FMath::FInterpConstantTo(Current, Target, DeltaTime, Current < Target ? IncreaseSpeed : DecreaseSpeed);
}
