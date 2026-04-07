// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChunkWorld.generated.h"

UCLASS()
class WICKEDWIZARDWASHDOWN_API AChunkWorld : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChunkWorld();

public:
	UPROPERTY(EditAnywhere, Category="Chunk World")
	TSubclassOf<AActor> Chunk;

	UPROPERTY(EditAnywhere, Category="Chunk World")
	int DrawDistance = 5;
	
	UPROPERTY(EditAnywhere, Category="Chunk World")
	int ChunkSize = 32;
	
	UPROPERTY(EditAnywhere, Category="Chunk World")
	bool bGenerateXAxis = true;
	
	UPROPERTY(EditAnywhere, Category="Chunk World")
	bool bGenerateYAxis = true;
	
	UPROPERTY(EditAnywhere, Category="Chunk World")
	bool bGenerateZAxis = true;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
