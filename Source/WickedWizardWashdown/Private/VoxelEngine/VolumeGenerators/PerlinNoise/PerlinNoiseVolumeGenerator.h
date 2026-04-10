// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VoxelEngine/VolumeGenerators/VolumeGenerator.h"
#include "PerlinNoiseVolumeGenerator.generated.h"

/**
 * 
 */
UCLASS()
class WICKEDWIZARDWASHDOWN_API UPerlinNoiseVolumeGenerator : public UVolumeGenerator
{
	GENERATED_BODY()
	
public:
	virtual void OnConstruct_Implementation() override;
	
	virtual float Step_Implementation(float X, float Y, float Z, float Value, AChunkBase* Chunk, FVector ChunkPosition) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bGenerateXAxis;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bGenerateYAxis;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bGenerateZAxis;
	
private:
	FVector BooleanAxisVector;
};
