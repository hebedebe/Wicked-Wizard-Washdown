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
	
	virtual class UWorld* GetWorld() const override;
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnConstruct(); // Used to precalculate values, especially ones that are not thread-safe
	
	UFUNCTION(BlueprintImplementableEvent, meta=(BlueprintThreadSafe))
	float Step(float X, float Y, float Z, float Value, AChunkBase* Chunk, FVector ChunkPosition);
	
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	float GetNoise(FVector Position) const;
	
	
public:
	FastNoiseLite* Noise;
	
public:
	UPROPERTY(EditAnywhere, Category="Noise")
	double NoiseFrequency = 0.06f;
	
private:
	int Seed;
};
