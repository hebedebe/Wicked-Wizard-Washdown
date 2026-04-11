// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs/ChunkFormat.h"
#include "GameFramework/Actor.h"
#include "ChunkWorld.generated.h"

class UVolumeGenerator;
class AChunkBase;

UCLASS()
class WICKEDWIZARDWASHDOWN_API AChunkWorld : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChunkWorld();
	
	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, Category="Chunk World")
	TSubclassOf<AChunkBase> ChunkClass;
	
	UPROPERTY(EditAnywhere, Category="Chunk World")
	TArray<TSubclassOf<UVolumeGenerator>> VolumeGeneratorClasses;

	UPROPERTY(EditAnywhere, Category="Chunk World")
	FIntVector GenerationDistance = FIntVector(1,1,1);
	
	UPROPERTY(EditAnywhere, Category="Chunk World")
	FChunkFormat ChunkFormat;
	
	UPROPERTY(EditAnywhere, Category="Chunk World")
	int MaxChunksGeneratedPerTick = 1;

public:
	UFUNCTION(BlueprintCallable)
	void RebuildDirtyChunks();
	
	UFUNCTION(BlueprintCallable)
	void MarkChunkDirty(AChunkBase* Chunk);
	
	UFUNCTION(BlueprintCallable)
	void SetVoxelValueInSphere(FVector WorldCenter, float Radius, float Value, FVector Scale=FVector(1,1,1), bool bAutoRebuild=true);
	
	UFUNCTION(BlueprintCallable)
	AChunkBase* GetChunkAtPosition(FVector WorldPosition);
	
	UFUNCTION(BlueprintCallable)
	void PropagateChunkBorderVoxels(AChunkBase* Chunk); // Updates the borders of the neighbouring chunks to match this one
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TMap<FIntVector, AChunkBase*> Chunks;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<UVolumeGenerator*> VolumeGenerators;
	
	UPROPERTY()
	TSet<AChunkBase*> DirtyChunks;
	
	UPROPERTY()
	TArray<AChunkBase*> ChunksToGenerate;
	
protected:
	int Seed;
};
