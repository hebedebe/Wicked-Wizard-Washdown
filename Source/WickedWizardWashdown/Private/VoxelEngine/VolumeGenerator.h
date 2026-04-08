// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "VolumeGenerator.generated.h"

class AChunkBase;
class FastNoiseLite;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class WICKEDWIZARDWASHDOWN_API UVolumeGenerator : public UObject
{
	GENERATED_BODY()
	
public:
	UVolumeGenerator();
	
public:
	virtual void BeginDestroy() override;
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	float Step(int X, int Y, int Z, float Value, AChunkBase* Chunk);
	
	UFUNCTION(BlueprintCallable)
	float GetNoise(FVector Position) const;
	
public:
	FastNoiseLite* Noise;
	
public:
	UPROPERTY(EditAnywhere, Category="Noise")
	double NoiseFrequency = 0.06f;
	
private:
	int Seed;
};
