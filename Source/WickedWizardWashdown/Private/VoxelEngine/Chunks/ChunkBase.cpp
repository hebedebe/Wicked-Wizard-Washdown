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

void AChunkBase::Generate()
{
	GenerateVolume();
	
	GenerateMesh();
	
	ApplyMesh();
}

// Called when the game starts or when spawned
void AChunkBase::BeginPlay()
{
	Super::BeginPlay();

	const int ChunkSize = ChunkFormat.CellsPerChunk;
	Voxels.SetNum(pow(ChunkSize + 1, 3));
}

void AChunkBase::GenerateVolume()
{
	const FVector ChunkPosition = GetActorLocation();
	const int ChunkSize = ChunkFormat.CellsPerChunk;
	const int AxisSize = ChunkSize + 1;
	const int AxisSizeSquared = AxisSize * AxisSize;
	const int TotalVoxels = AxisSize * AxisSize * AxisSize;
	const bool bHasGenerators = !VolumeGenerators.IsEmpty();
	
	ParallelFor(TotalVoxels, [&](const int32 i)
	{
		const int z = i / (AxisSizeSquared);
		const int y = (i / AxisSize) % AxisSize;
		const int x = i % AxisSize;

		float Voxel = 0.f;

		if (bHasGenerators)
		{
			for (UVolumeGenerator* VolumeGenerator : VolumeGenerators)
			{
				if (VolumeGenerator)
					Voxel = VolumeGenerator->Step(x, y, z, Voxel, this, ChunkPosition);
			}
		}
		
		Voxels[i] = Voxel;
	});
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
