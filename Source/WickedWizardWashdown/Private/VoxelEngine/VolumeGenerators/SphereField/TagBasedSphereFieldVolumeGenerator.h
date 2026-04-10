// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VoxelEngine/VolumeGenerators/VolumeGenerator.h"
#include "TagBasedSphereFieldVolumeGenerator.generated.h"

/**
 * 
 */
UCLASS()
class WICKEDWIZARDWASHDOWN_API UTagBasedSphereFieldVolumeGenerator : public UVolumeGenerator
{
	GENERATED_BODY()
	
public:
	virtual void OnConstruct_Implementation() override;
	
	virtual float Step_Implementation(float X, float Y, float Z, float Value, AChunkBase* Chunk, FVector ChunkPosition) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Tag = "NoWorldGen";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector AxisMultipliers = FVector::OneVector;
	
protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> Positions;
};
