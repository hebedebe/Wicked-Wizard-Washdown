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

float UVolumeGenerator::GetNoise(const FVector Position) const
{
	return Noise->GetNoise(Position.X, Position.Y, Position.Z);
}
