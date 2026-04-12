// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkWorld.h"

#include "VectorTypes.h"
#include "VolumeGenerators/VolumeGenerator.h"
#include "Chunks/ChunkBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AChunkWorld::AChunkWorld()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Make sure the world can actually unpause the world when its done
	SetTickableWhenPaused(true);
}

void AChunkWorld::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (ChunksToGenerate.IsEmpty()) return; // Our work is already done.
	
	// Pause game until chunk gen is finished (mainly to prevent physics updates)
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
	const double StartTime = FPlatformTime::Seconds();
	
	// Snapshot the set — MarkChunkDirty may add neighbours during the loop
	TArray<AChunkBase*> ToRebuild = DirtyChunks.Array();
	
	for (AChunkBase* Chunk : ToRebuild)
	{
		PropagateChunkBorderVoxels(Chunk);
	}
	
	// Now rebuild everything (including newly dirtied neighbours)
	for (AChunkBase* Chunk : DirtyChunks)
	{
		Chunk->GenerateMesh();
		Chunk->ApplyMesh();
		Chunk->bDirty = false;
	}
	
	const int BuiltChunks = DirtyChunks.Num();
	DirtyChunks.Empty();
	
	const double EndTime = FPlatformTime::Seconds();
	const double ElapsedTimeMs = (EndTime-StartTime) * 1000.0;
	UE_LOG(LogTemp, Log, TEXT("Rebuilt %i dirty chunks in %f ms."), BuiltChunks, ElapsedTimeMs);
}

void AChunkWorld::MarkChunkDirty(AChunkBase* Chunk)
{
	if (Chunk->bDirty) return;
	DirtyChunks.Add(Chunk);	
	Chunk->bDirty = true;
}

void AChunkWorld::SetVoxelValueInSphere(const FVector WorldCenter, const float Radius, const float Value, 
	const bool bAutoRebuild)
{
	TArray<FIntVector> Keys;
	TQueue<AChunkBase*, EQueueMode::Mpsc> ModifiedChunks;
	
	Chunks.GetKeys(Keys);
	
	ParallelFor(Chunks.Num(), [&](const int32 Index)
	{
		AChunkBase* Chunk = Chunks[Keys[Index]];
		if (Chunk->SetVoxelValueInSphere(WorldCenter, Radius, Value))
		{
			// Queue non-thread safe functions for execution outside of the thread
			ModifiedChunks.Enqueue(Chunk); 
		}
	}, EParallelForFlags::Unbalanced);
	
	AChunkBase* Chunk;
	while (ModifiedChunks.Dequeue(Chunk))
	{
		// Execute these outside of the threads to prevent overlapping writes to the dirty set
		MarkChunkDirty(Chunk);
		PropagateChunkBorderVoxels(Chunk);
	}
	
	if (bAutoRebuild)
		RebuildDirtyChunks();
}

void AChunkWorld::SetVoxelValueInCylinder(const FVector WorldCenter, const float Radius, 
	const float HalfHeight, const EAxis::Type Axis, const float Value, const bool bAutoRebuild)
{
	TArray<FIntVector> Keys;
	TQueue<AChunkBase*, EQueueMode::Mpsc> ModifiedChunks;
	
	Chunks.GetKeys(Keys);
	
	ParallelFor(Chunks.Num(), [&](const int32 Index)
	{
		AChunkBase* Chunk = Chunks[Keys[Index]];
		const bool bModified = Chunk->SetVoxelValueInCylinder(WorldCenter, Radius, HalfHeight, Axis, Value);
		if (bModified)
		{
			// Queue non-thread safe functions for execution outside of the thread
			ModifiedChunks.Enqueue(Chunk); 
		}
	}, EParallelForFlags::Unbalanced);
	
	AChunkBase* Chunk;
	while (ModifiedChunks.Dequeue(Chunk))
	{
		// Execute these outside of the threads to prevent overlapping writes to the dirty set
		MarkChunkDirty(Chunk);
		PropagateChunkBorderVoxels(Chunk);
	}
	
	if (bAutoRebuild)
		RebuildDirtyChunks();
}

AChunkBase* AChunkWorld::GetChunkAtPosition(FVector WorldPosition)
{
	const FIntVector AdjustedPosition = FIntVector(WorldPosition/(ChunkFormat.CellSize*ChunkFormat.CellsPerChunk));
	if (Chunks.Contains(AdjustedPosition))
	{
		return Chunks[AdjustedPosition];
	}
	
	UE_LOG(LogTemp, Warning, 
		TEXT("GetChunkAtPosition() called outside of chunk bounds (%i, %i, %i), (%f, %f, %f), returning nullptr."), 
		AdjustedPosition.X, AdjustedPosition.Y, AdjustedPosition.Z, 
		WorldPosition.X, WorldPosition.Y, WorldPosition.Z);
	return nullptr;
}

void AChunkWorld::PropagateChunkBorderVoxels(AChunkBase* Chunk)
{
	const int N = ChunkFormat.CellsPerChunk;

	// Get this chunk's index from the map
	FIntVector ChunkIndex;
	for (const auto& Pair : Chunks)
	{
		if (Pair.Value == Chunk) { ChunkIndex = Pair.Key; break; }
	}

	// For each positive-face neighbour (+X, +Y, +Z)
	const FIntVector Dirs[3] = { {1,0,0}, {0,1,0}, {0,0,1} };

	for (const FIntVector& Dir : Dirs)
	{
		AChunkBase** NeighbourPtr = Chunks.Find(ChunkIndex + Dir);
		if (!NeighbourPtr) continue;
		AChunkBase* Neighbour = *NeighbourPtr;

		for (int A = 0; A <= N; A++)
		{
			for (int B = 0; B <= N; B++)
			{
				if (Dir.X) // Copy our X=N slice into neighbour's X=0 column
				{
					Neighbour->Voxels[Neighbour->GetVoxelIndex(0, A, B)] =
						Chunk->Voxels[Chunk->GetVoxelIndex(N, A, B)];
				}
				else if (Dir.Y)
				{
					Neighbour->Voxels[Neighbour->GetVoxelIndex(A, 0, B)] =
						Chunk->Voxels[Chunk->GetVoxelIndex(A, N, B)];
				}
				else // Dir.Z
				{
					Neighbour->Voxels[Neighbour->GetVoxelIndex(A, B, 0)] =
						Chunk->Voxels[Chunk->GetVoxelIndex(A, B, N)];
				}
			}
		}

		// Neighbour's mesh now depends on our data — mark it dirty too
		MarkChunkDirty(Neighbour);
	}
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
				Chunk->OwningWorld = this;
				
				Chunks.Add(FIntVector(x, y, z), Chunk);
			}
		}
	}
	
	// Mark all the chunks we just created to be generated
	Chunks.GenerateValueArray(ChunksToGenerate);
}
