// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkWorld.h"


// Sets default values
AChunkWorld::AChunkWorld()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AChunkWorld::BeginPlay()
{
	Super::BeginPlay();

	for (int x = -DrawDistance * bGenerateXAxis; x < (DrawDistance-1) * bGenerateXAxis + 1; ++x)
	{
		for (int y = -DrawDistance * bGenerateYAxis; y < (DrawDistance-1) * bGenerateYAxis + 1; ++y)
		{
			for (int z = -DrawDistance * bGenerateZAxis; z < (DrawDistance-1) * bGenerateZAxis + 1; ++z)
			{
				GetWorld()->SpawnActor<AActor>(Chunk, 
					FVector(x * ChunkSize * 100, y * ChunkSize * 100, z * ChunkSize * 100),
					FRotator::ZeroRotator);
			}
		}
		
	}
}
