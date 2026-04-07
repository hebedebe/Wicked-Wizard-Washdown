// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChunkMeshData.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "ChunkBase.generated.h"

class FastNoiseLite;
class UProceduralMeshComponent;

UCLASS()
class WICKEDWIZARDWASHDOWN_API AChunkBase : public AActor
{
	GENERATED_BODY()

public:
	AChunkBase();

public:
	UPROPERTY(EditDefaultsOnly, Category="Chunk")
	int Size = 64;
	
	UPROPERTY(EditDefaultsOnly, Category="Chunk")
	float CellScale = 100.f;
	
	UPROPERTY(EditDefaultsOnly, Category="HeightMap")
	float Frequency = 0.03f;
	
	UPROPERTY(EditDefaultsOnly, Category="Chunk", BlueprintReadWrite)
	TObjectPtr<UProceduralMeshComponent> Mesh;
	
public:
	int Seed = 0;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void GenerateHeightMap();
	
	virtual void GenerateMesh();
	
protected:
	FastNoiseLite* Noise;
	FChunkMeshData MeshData;
	int VertexCount = 0;
	
private:
	void ApplyMesh() const;
};
