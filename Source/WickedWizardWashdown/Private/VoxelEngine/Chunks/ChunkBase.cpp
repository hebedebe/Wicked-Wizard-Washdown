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
	// const auto Position = GetActorLocation() / CellScale;
	int ChunkSize = ChunkFormat.CellsPerChunk;
	
	for (int x = 0; x <= ChunkSize; x++)
	{
		for (int y = 0; y <= ChunkSize; y++)
		{
			for (int z = 0; z <= ChunkSize; z++)
			{
				const auto Index = GetVoxelIndex(x, y, z);
				Voxels[Index] = 0.f; // Most functions will be additive, so make sure to start with a consistent value
				for (const auto VolumeGenerator : VolumeGenerators)
				{
					Voxels[Index] = VolumeGenerator->Step(x, y, z, Voxels[Index], this); 
				}
				
				//
				// Voxels[Index] = Noise->GetNoise(0.0, y + Position.Y, z + Position.Z) 
				// - powf((Position.X + x) * 0.1, 2)
				// + std::max(0.0, UE::Geometry::Distance(FVector(0,0,0), (Position + FVector(x,y,z)) / FVector(1,1.3,1)) - 100) * 0.3;
				
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
