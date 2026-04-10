// Fill out your copyright notice in the Description page of Project Settings.


#include "TagBasedSphereFieldVolumeGenerator.h"

#include "Kismet/GameplayStatics.h"
#include "VoxelEngine/Chunks/ChunkBase.h"

void UTagBasedSphereFieldVolumeGenerator::OnConstruct_Implementation()
{
	Super::OnConstruct_Implementation();
	
	TArray<AActor*> TaggedActors;
	UGameplayStatics::GetAllActorsWithTag(this, Tag, TaggedActors);
	
	for (const AActor* Actor : TaggedActors)
	{
		Positions.Add(Actor->GetActorLocation());
	}
}

float UTagBasedSphereFieldVolumeGenerator::Step_Implementation(float X, float Y, float Z, float Value,
                                                               AChunkBase* Chunk, FVector ChunkPosition)
{
	const FVector Voxel = (FVector(X, Y, Z) * Chunk->ChunkFormat.CellSize + ChunkPosition) * AxisMultipliers;
	const float RadiusSquared = Radius * Radius;

	for (const FVector& Pos : Positions)
	{
		const float DistanceSquared = FVector::DistSquared(Voxel, Pos*AxisMultipliers);
		if (DistanceSquared < RadiusSquared)
			return (RadiusSquared - DistanceSquared) * -1.f;
	}

	return Value;
}
