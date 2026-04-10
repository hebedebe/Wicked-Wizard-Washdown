#include "ChunkBase.h"

#include "VoxelEngine/VolumeGenerators/VolumeGenerator.h"

#define PROFILE(contents, message, name, additionalArgs) \
	const double name ## StartTime = FPlatformTime::Seconds(); \
	contents \
	const double name ## EndTime = FPlatformTime::Seconds(); \
	const double name ## TimeMS = ((name ## EndTime) - (name ## StartTime)) * 1000.0; \
	UE_LOG(LogTemp, Warning, TEXT(message), (name ## TimeMS), additionalArgs); \


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
	const int ChunkSize = ChunkFormat.CellsPerChunk + 1;
	return Z * ChunkSize * ChunkSize + Y * ChunkSize + X;
}

void AChunkBase::Generate()
{
	static int ChunksGenerated = 0;
	
	PROFILE(GenerateVolume();, "GenerateVolume took %f ms for one chunk (id %i)", one, ChunksGenerated)
	
	PROFILE(GenerateMesh();, "GenerateMesh took %f ms for one chunk (id %i)", two, ChunksGenerated)
	
	PROFILE(ApplyMesh();, "ApplyMesh took %f ms for one chunk (id %i)", three, ChunksGenerated)
	
	ChunksGenerated++;
}

// Called when the game starts or when spawned
void AChunkBase::BeginPlay()
{
	Super::BeginPlay();

	const int ChunkSize = ChunkFormat.CellsPerChunk + 1;
	const int TotalVoxels = ChunkSize * ChunkSize * ChunkSize;
	Voxels.SetNum(TotalVoxels);
}

void AChunkBase::GenerateVolume()
{
	const FVector ChunkPosition = GetActorLocation();
	const int ChunkSize = ChunkFormat.CellsPerChunk;
	const int AxisSize = ChunkSize + 1;
	const int AxisSizeSquared = AxisSize * AxisSize;
	const int TotalVoxels = AxisSizeSquared * AxisSize;
	
	ParallelFor(TotalVoxels, [&](const int32 i)
	{
		const int z = i / (AxisSizeSquared);
		const int y = (i / AxisSize) % AxisSize;
		const int x = i % AxisSize;

		float Voxel = 0.f;
		
		for (UVolumeGenerator* VolumeGenerator : VolumeGenerators)
		{
			if (VolumeGenerator)
				Voxel = VolumeGenerator->Step(x, y, z, Voxel, this, ChunkPosition);
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
