// Fill out your copyright notice in the Description page of Project Settings.

#include "VolumeGenerator.h"


UVolumeGenerator::UVolumeGenerator()
{
	Seed = rand();
	
	Noise = MakeUnique<FastNoiseLite>();
	Noise->SetFrequency(NoiseFrequency);
	Noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	Noise->SetFractalType(FastNoiseLite::FractalType_FBm);
	Noise->SetSeed(Seed);
}

void UVolumeGenerator::OnConstruct_Implementation()
{
}

UWorld* UVolumeGenerator::GetWorld() const
{
	const UObject *Outer = GetOuter();
	if (IsValid(Outer) && Outer->IsA<AActor>() && !Outer->HasAnyFlags(RF_ClassDefaultObject))
	{
		return Outer->GetWorld();
	}

	return nullptr;
}

float UVolumeGenerator::Step_Implementation(float X, float Y, float Z, float Value, AChunkBase* Chunk,
	FVector ChunkPosition)
{
	return Value;
}

float UVolumeGenerator::GetNoise(const FVector Position) const
{
	const FVector FrequencyAdjustedPosition = Position*NoiseFrequency;
	return Noise->GetNoise(FrequencyAdjustedPosition.X, FrequencyAdjustedPosition.Y, FrequencyAdjustedPosition.Z);
}
