// Fill out your copyright notice in the Description page of Project Settings.


#include "PerlinNoiseVolumeGenerator.h"

#include "VoxelEngine/Chunks/ChunkBase.h"

void UPerlinNoiseVolumeGenerator::OnConstruct_Implementation()
{
	BooleanAxisVector = FVector(
		bGenerateXAxis ? 1.f : 0.f,
		bGenerateYAxis ? 1.f : 0.f,
		bGenerateZAxis ? 1.f : 0.f
	);
}

float UPerlinNoiseVolumeGenerator::Step_Implementation(const float X, const float Y, const float Z, const float Value, 
	AChunkBase* Chunk, const FVector ChunkPosition)
{
	if (!IsValid(Chunk))
		return 0.f;
	
	const FVector VoxelPosition = FVector(X, Y, Z) * Chunk->ChunkFormat.CellSize + ChunkPosition;
	const FVector AxisModifiedPosition = VoxelPosition * BooleanAxisVector;

	const float PerlinValue = GetNoise(AxisModifiedPosition);
	
	return Value + PerlinValue;
}
