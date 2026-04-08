// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChunkWorld.generated.h"

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
	FIntVector DrawDistance = FIntVector(1,1,1);
	
	UPROPERTY(EditAnywhere, Category="Chunk World")
	int ChunkSize = 32;
	
	UPROPERTY(EditAnywhere, Category="Chunk World")
	float CellScale = 100.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TMap<FIntVector, AChunkBase*> Chunks;
	
protected:
	int Seed;
};
