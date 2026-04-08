// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chunks/ChunkFormat.h"
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

public:
	UPROPERTY(EditAnywhere, Category="Chunk World")
	TSubclassOf<AChunkBase> ChunkClass;
	
	UPROPERTY(EditAnywhere, Category="Chunk World")
	TArray<TSubclassOf<UVolumeGenerator>> VolumeGeneratorClasses;

	UPROPERTY(EditAnywhere, Category="Chunk World")
	FIntVector GenerationDistance = FIntVector(1,1,1);
	
	UPROPERTY(EditAnywhere, Category="Chunk World")
	FChunkFormat ChunkFormat;

public:
	UFUNCTION(BlueprintCallable)
	void RebuildDirtyChunks();
	
	UFUNCTION(BlueprintCallable)
	void MarkChunkDirty(AChunkBase* Chunk);
	
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
protected:
	int Seed;
	
};
