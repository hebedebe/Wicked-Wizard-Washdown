// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChunkFormat.h"
#include "../ChunkMeshData.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "ChunkBase.generated.h"

class UVolumeGenerator;
class FastNoiseLite;
class UProceduralMeshComponent;

UCLASS()
class WICKEDWIZARDWASHDOWN_API AChunkBase : public AActor
{
	GENERATED_BODY()

	friend class AChunkWorld;
	
public:
	AChunkBase();

public:
	UFUNCTION(BlueprintCallable)
	int GetVoxelIndex(int X, int Y, int Z) const;
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Chunk", BlueprintReadWrite)
	TObjectPtr<UProceduralMeshComponent> Mesh;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void GenerateVolume();
	
	virtual void GenerateMesh();
	
protected:
	FChunkMeshData MeshData;
	
	int VertexCount = 0;
	
	bool bDirty = false;
	
	TArray<float> Voxels;
	
protected:
	UPROPERTY()
	TArray<UVolumeGenerator*> VolumeGenerators;
	
	UPROPERTY(BlueprintReadOnly, Category="Chunk")
	FChunkFormat ChunkFormat;
	
private:
	void ApplyMesh() const;
};
