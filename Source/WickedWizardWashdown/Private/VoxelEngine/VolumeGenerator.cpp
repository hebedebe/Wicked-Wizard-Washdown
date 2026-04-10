// Fill out your copyright notice in the Description page of Project Settings.

#include "VolumeGenerator.h"

#include "../External/FastNoiseLite/FastNoiseLite.h"

UVolumeGenerator::UVolumeGenerator()
{
	Seed = rand();
	
	Noise = new FastNoiseLite();
	
	Noise->SetFrequency(NoiseFrequency);
	Noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	Noise->SetFractalType(FastNoiseLite::FractalType_FBm);
	Noise->SetSeed(Seed);
}

void UVolumeGenerator::BeginDestroy()
{
	UObject::BeginDestroy();
	
	delete Noise;
	Noise = nullptr;
}

class UWorld* UVolumeGenerator::GetWorld() const
{
	UObject *outer = GetOuter();
	if (outer && outer->IsA<AActor>() && !outer->HasAnyFlags(RF_ClassDefaultObject))
	{
		return outer->GetWorld();
	}

	return nullptr;
}

float UVolumeGenerator::GetNoise(const FVector Position) const
{
	const FVector FrequencyAdjustedPosition = Position*NoiseFrequency;
	return Noise->GetNoise(FrequencyAdjustedPosition.X, FrequencyAdjustedPosition.Y, FrequencyAdjustedPosition.Z);
}
