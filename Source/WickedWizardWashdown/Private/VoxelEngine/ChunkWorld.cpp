// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkWorld.h"

#include "VolumeGenerator.h"
#include "Chunks/ChunkBase.h"
#include "Misc/LowLevelTestAdapter.h"
#include "Misc/RuntimeErrors.h"


// Sets default values
AChunkWorld::AChunkWorld()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AChunkWorld::RebuildDirtyChunks()
{
	if (DirtyChunks.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No chunks to rebuild"));
		return;
	}
	
	for (AChunkBase* Chunk : DirtyChunks)
	{
		Chunk->GenerateMesh();
		Chunk->ApplyMesh();
		Chunk->bDirty = false;

		const auto ChunkLocation = Chunk->GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("Rebuilt chunk at [%f, %f, %f]"), ChunkLocation.X, ChunkLocation.Y, ChunkLocation.Z);
	}
	
	DirtyChunks.Empty();
}

void AChunkWorld::MarkChunkDirty(AChunkBase* Chunk)
{
	if (Chunk->bDirty) return;
	DirtyChunks.Add(Chunk);
	Chunk->bDirty = true;
}

// Called when the game starts or when spawned
void AChunkWorld::BeginPlay()
{
	Super::BeginPlay();

	CHECK(IsValid(ChunkClass));
	
	CHECK(!VolumeGeneratorClasses.IsEmpty());
	for (auto VolumeGeneratorClass : VolumeGeneratorClasses)
	{
		CHECK(IsValid(ChunkClass));
		VolumeGenerators.Add(NewObject<UVolumeGenerator>(this, VolumeGeneratorClass));
	}
	
	
	for (int x = -GenerationDistance.X; x < GenerationDistance.X; ++x)
	{
		for (int y = -GenerationDistance.Y; y < GenerationDistance.Y; ++y)
		{
			for (int z = -GenerationDistance.Z; z < GenerationDistance.Z; ++z)
			{
				const int ChunkSize = ChunkFormat.CellsPerChunk;
				const int CellSize = ChunkFormat.CellSize;
				auto ChunkPosition = FVector(x * ChunkSize * CellSize, y * ChunkSize * CellSize, z * ChunkSize * CellSize);
				auto ChunkRotation = FRotator::ZeroRotator;
				
				const auto Chunk = GetWorld()->SpawnActorDeferred<AChunkBase>(ChunkClass, FTransform(ChunkRotation, ChunkPosition));
				
				Chunk->ChunkFormat = ChunkFormat;
				Chunk->VolumeGenerators = VolumeGenerators;
				
				Chunks.Add(FIntVector(x, y, z), Chunk);
			}
		}
		
	}
}
