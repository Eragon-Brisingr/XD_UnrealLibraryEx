// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_TextureFunctionLibrary.h"
#include <Engine/Texture2D.h>
#include <TextureResource.h>
#include <BulkData.h>
#include <Engine/TextureRenderTarget2D.h>
#include "XD_UnrealLibraryExUtility.h"

float UXD_TextureFunctionLibrary::GetPixelBrightness(const FColor& Pixel)
{
	// Use a formula to determine brightness based on pixel color values. Source for Formula used:
	// www.stackoverflow.com/questions/596216/formula-to-determine-brightness-of-rgb-color
	return (0.299f * Pixel.R) + (0.587f * Pixel.G) + (0.114f * Pixel.B);
}

float UXD_TextureFunctionLibrary::GetPixelsMaxBrightness(const TArray<FColor>& Pixels)
{
	float MaxBrightness = 0.f;
	for (const FColor& Pixel : Pixels) 
	{
		MaxBrightness = FMath::Max(MaxBrightness, GetPixelBrightness(Pixel));
	}
	return MaxBrightness;
}