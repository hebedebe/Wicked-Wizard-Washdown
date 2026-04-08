// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkWorld.h"

#include "Chunks/ChunkBase.h"
#include "Misc/LowLevelTestAdapter.h"
#include "Misc/RuntimeErrors.h"


// Sets default values
AChunkWorld::AChunkWorld()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AChunkWorld::BeginPlay()
{
	Super::BeginPlay();

	CHECK(IsValid(ChunkClass));
	
	Seed = rand();
	
	UE_LOG(LogTemp, Warning, TEXT("Calculated seed: %i"), Seed);
	
	
	for (int x = -DrawDistance.X; x < DrawDistance.X; ++x)
	{
		for (int y = -DrawDistance.Y; y < DrawDistance.Y; ++y)
		{
			for (int z = -DrawDistance.Z; z < DrawDistance.Z; ++z)
			{
				auto ChunkPosition = FVector(x * ChunkSize * CellScale, y * ChunkSize * CellScale, z * ChunkSize * CellScale);
				auto ChunkRotation = FRotator::ZeroRotator;
				
				const auto Chunk = GetWorld()->SpawnActorDeferred<AChunkBase>(ChunkClass, FTransform(ChunkRotation, ChunkPosition));
				
				Chunk->Seed = this->Seed;
				Chunk->CellScale = this->CellScale;
				
				Chunks[{x, y, z}] = Chunk;
			}
		}
		
	}
}
