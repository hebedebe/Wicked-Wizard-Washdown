// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkWorld.h"

#include "VolumeGenerators/VolumeGenerator.h"
#include "Chunks/ChunkBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AChunkWorld::AChunkWorld()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetTickableWhenPaused(true);
}

void AChunkWorld::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (ChunksToGenerate.IsEmpty()) return; // Our work is already done.
	
	// Pause game until worldgen is finished (mainly to prevent physics updates)
	if (!ChunksToGenerate.IsEmpty())
		UGameplayStatics::SetGamePaused(this, true);
	
	int ChunksGenerated = 0;
	while (ChunksGenerated < MaxChunksGeneratedPerTick && !ChunksToGenerate.IsEmpty()) {
		ChunksToGenerate[ChunksToGenerate.Num()-ChunksGenerated-1]->Generate();
		ChunksGenerated++;
		if (ChunksGenerated >= ChunksToGenerate.Num()) break;
	}
	
	ChunksToGenerate.SetNum(ChunksToGenerate.Num()-ChunksGenerated);
	
	UE_LOG(LogTemp, Log, TEXT("Chunks generated: %d"), ChunksGenerated);
	
	if (ChunksGenerated > 0 && ChunksToGenerate.IsEmpty())
	{
		UGameplayStatics::SetGamePaused(this, false);
	}
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

	// Verify that we haven't left the chunk class field empty
	if (!IsValid(ChunkClass))
	{
		UE_LOG(LogTemp, Error, TEXT("Chunk class is invalid."));
	}
	
	for (auto VolumeGeneratorClass : VolumeGeneratorClasses)
	{
		if (IsValid(VolumeGeneratorClass)) // Make sure we don't fill the arrays we pass to the chunks with junk.
		{
			UVolumeGenerator* Generator = NewObject<UVolumeGenerator>(this, VolumeGeneratorClass);
			Generator->OnConstruct();
			VolumeGenerators.Add(Generator);
		}
	}
	
	// Spawn the chunk actors we will be generating
	for (int x = -GenerationDistance.X; x < GenerationDistance.X; ++x)
	{
		for (int y = -GenerationDistance.Y; y < GenerationDistance.Y; ++y)
		{
			for (int z = -GenerationDistance.Z; z < GenerationDistance.Z; ++z)
			{
				const int ChunkSize = ChunkFormat.CellsPerChunk;
				const int CellSize = ChunkFormat.CellSize;
				FVector ChunkPosition = FVector(x * ChunkSize * CellSize, y * ChunkSize * CellSize, z * ChunkSize * CellSize);
				FRotator ChunkRotation = FRotator::ZeroRotator;
				
				const auto Chunk = GetWorld()->SpawnActor<AChunkBase>(ChunkClass, FTransform(ChunkRotation, ChunkPosition));
				
				Chunk->ChunkFormat = ChunkFormat;
				Chunk->VolumeGenerators = VolumeGenerators;
				
				Chunks.Add(FIntVector(x, y, z), Chunk);
			}
		}
	}
	
	// Mark all the chunks we just created to be generated
	Chunks.GenerateValueArray(ChunksToGenerate);
}
