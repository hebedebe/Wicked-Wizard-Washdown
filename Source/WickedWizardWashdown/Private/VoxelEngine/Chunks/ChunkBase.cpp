#include "ChunkBase.h"

#include "External/FastNoiseLite/FastNoiseLite.h"
#include "VoxelEngine/VolumeGenerator.h"


AChunkBase::AChunkBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	
	// Mesh Settings
	Mesh->SetCastShadow(false);
	
	// Set Mesh as root
	SetRootComponent(Mesh);
}

int AChunkBase::GetVoxelIndex(const int X, const int Y, const int Z) const
{
	int ChunkSize = ChunkFormat.CellsPerChunk;
	return Z * (ChunkSize + 1) * (ChunkSize + 1) + Y * (ChunkSize + 1) + X;
}

// Called when the game starts or when spawned
void AChunkBase::BeginPlay()
{
	Super::BeginPlay();

	int ChunkSize = ChunkFormat.CellsPerChunk;
	Voxels.SetNum(pow(ChunkSize + 1, 3));
	
	GenerateVolume();
	
	GenerateMesh();
	
	ApplyMesh();
}

void AChunkBase::GenerateVolume()
{
	const int ChunkSize = ChunkFormat.CellsPerChunk;
	
	for (int x = 0; x <= ChunkSize; x++)
	{
		for (int y = 0; y <= ChunkSize; y++)
		{
			for (int z = 0; z <= ChunkSize; z++)
			{
				const auto Index = GetVoxelIndex(x, y, z);
				
				// Most functions perform additive operations, so we start with a consistent value instead of unwritten memory
				Voxels[Index] = 0.f; 
				
				for (const auto VolumeGenerator : VolumeGenerators)
				{
					Voxels[Index] = VolumeGenerator->Step(x, y, z, Voxels[Index], this); 
				}
			}
		}
	}
}

void AChunkBase::GenerateMesh()
{
}

void AChunkBase::ApplyMesh() const
{
	Mesh->ClearMeshSection(0);
	Mesh->CreateMeshSection(0, MeshData.Vertices, MeshData.Triangles, MeshData.Normals, MeshData.UV0, 
		TArray<FColor>(), TArray<FProcMeshTangent>(), true);
}
