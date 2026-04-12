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

bool AChunkBase::SetVoxelValueInSphere(const FVector WorldCenter, const float Radius, const float Value)
{
	const FVector ChunkPosition = GetActorLocation();
	const int CellSize = ChunkFormat.CellSize;
	const int ChunkSize = ChunkFormat.CellsPerChunk;

	// Convert world-space center to this chunk's local voxel space
	const FVector LocalCenter = (WorldCenter - ChunkPosition) / CellSize;
	const float RadiusInVoxels = Radius / CellSize;
	const float RadiusSq = RadiusInVoxels * RadiusInVoxels;

	// Clamp iteration bounds to the voxels that could be affected
	const int MinX = FMath::Clamp(FMath::FloorToInt(LocalCenter.X - RadiusInVoxels), 0, ChunkSize);
	const int MaxX = FMath::Clamp(FMath::CeilToInt(LocalCenter.X  + RadiusInVoxels), 0, ChunkSize);
	const int MinY = FMath::Clamp(FMath::FloorToInt(LocalCenter.Y - RadiusInVoxels), 0, ChunkSize);
	const int MaxY = FMath::Clamp(FMath::CeilToInt(LocalCenter.Y  + RadiusInVoxels), 0, ChunkSize);
	const int MinZ = FMath::Clamp(FMath::FloorToInt(LocalCenter.Z - RadiusInVoxels), 0, ChunkSize);
	const int MaxZ = FMath::Clamp(FMath::CeilToInt(LocalCenter.Z  + RadiusInVoxels), 0, ChunkSize);

	bool bModified = false;

	for (int x = MinX; x <= MaxX; x++)
	{
		for (int y = MinY; y <= MaxY; y++)
		{
			for (int z = MinZ; z <= MaxZ; z++)
			{
				const FVector Delta = FVector(x, y, z) - LocalCenter;
				if (Delta.SizeSquared() <= RadiusSq)
				{
					Voxels[GetVoxelIndex(x, y, z)] = Value;
					
					if (!bModified) //Add a check to skip unnecessary writes on many threads
						bModified = true;
				}
			}
		}
	}

	return bModified;
}

bool AChunkBase::SetVoxelValueInCylinder(const FVector WorldCenter, const float Radius, const float HalfHeight,
	const EAxis::Type Axis, const float Value)
{
    const FVector ChunkPosition = GetActorLocation();
    const int CellSize = ChunkFormat.CellSize;
    const int ChunkSize = ChunkFormat.CellsPerChunk;

    const FVector LocalCenter = (WorldCenter - ChunkPosition) / CellSize;
    const float RadiusInVoxels = Radius / CellSize;
    const float HalfHeightInVoxels = HalfHeight / CellSize;
    const float RadiusSquared = RadiusInVoxels * RadiusInVoxels;

    // Per-axis iteration bounds: radial axes use Radius, axial axis uses HalfHeight
    auto RadialBounds = [&](float Center, float R) -> TPair<int,int>
    {
        return {
            FMath::Clamp(FMath::FloorToInt(Center - R), 0, ChunkSize),
            FMath::Clamp(FMath::CeilToInt (Center + R), 0, ChunkSize)
        };
    };

    auto [MinX, MaxX] = RadialBounds(LocalCenter.X, Axis == EAxis::X ? HalfHeightInVoxels : RadiusInVoxels);
    auto [MinY, MaxY] = RadialBounds(LocalCenter.Y, Axis == EAxis::Y ? HalfHeightInVoxels : RadiusInVoxels);
    auto [MinZ, MaxZ] = RadialBounds(LocalCenter.Z, Axis == EAxis::Z ? HalfHeightInVoxels : RadiusInVoxels);

    bool bModified = false;

    for (int x = MinX; x <= MaxX; x++)
    {
        for (int y = MinY; y <= MaxY; y++)
        {
            for (int z = MinZ; z <= MaxZ; z++)
            {
                const FVector Delta = FVector(x, y, z) - LocalCenter;

                // Decompose delta into axial and radial components
                float AxialDistance;
                float RadialSquare;

                switch (Axis)
                {
                    case EAxis::X:
                        AxialDistance = FMath::Abs(Delta.X);
                        RadialSquare  = Delta.Y * Delta.Y + Delta.Z * Delta.Z;
                        break;
                    case EAxis::Y:
                        AxialDistance = FMath::Abs(Delta.Y);
                        RadialSquare  = Delta.X * Delta.X + Delta.Z * Delta.Z;
                        break;
                    case EAxis::Z:
                        AxialDistance = FMath::Abs(Delta.Z);
                        RadialSquare  = Delta.X * Delta.X + Delta.Y * Delta.Y;
                        break;
                	case default: // This shouldn't trigger
                		UE_LOG(LogTemp, Error, TEXT("Unspecified Axis!"));
						return false;
                	
                }

                if (RadialSquare <= RadiusSquared && AxialDistance <= HalfHeightInVoxels)
                {
                    Voxels[GetVoxelIndex(x, y, z)] = Value;
                    if (!bModified)
                        bModified = true;
                }
            }
        }
    }

    return bModified;
}

FIntVector AChunkBase::GetVoxelPositionFromIndex(const int Index) const
{
	const int AxisSize = ChunkFormat.CellsPerChunk + 1;

	const int z = Index / (AxisSize*AxisSize);
	const int y = (Index / AxisSize) % AxisSize;
	const int x = Index % AxisSize;
	
	return {x, y, z};
}

float AChunkBase::GetVoxelSafe(const int X, const int Y, const int Z) const
{
	const int AxisSize = ChunkFormat.CellsPerChunk + 1;
	if (X < 0 || Y < 0 || Z < 0 || X >= AxisSize || Y >= AxisSize || Z >= AxisSize)
		return 0.f;
	return Voxels[GetVoxelIndex(X, Y, Z)];
}

void AChunkBase::ResetMeshData()
{
    MeshData = FChunkMeshData();
    VertexCount = 0;
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
	
	ParallelFor(TotalVoxels, [&](const int32 ThreadIndex)
	{
		const int z = ThreadIndex / (AxisSizeSquared);
		const int y = (ThreadIndex / AxisSize) % AxisSize;
		const int x = ThreadIndex % AxisSize;

		float Voxel = 0.f;
		
		for (UVolumeGenerator* VolumeGenerator : VolumeGenerators)
		{
			if (VolumeGenerator)
				Voxel = VolumeGenerator->Step(x, y, z, Voxel, this, ChunkPosition);
		}
		
		Voxels[ThreadIndex] = Voxel;
	});
}

void AChunkBase::GenerateMesh()
{
	ResetMeshData(); // Here so that derived classes call this automatically as Super::GenerateMesh()
}

void AChunkBase::ApplyMesh() const
{
	Mesh->bUseAsyncCooking = true;
	Mesh->ClearMeshSection(0);
	Mesh->CreateMeshSection(0, MeshData.Vertices, MeshData.Triangles, MeshData.Normals, MeshData.UV0, 
		TArray<FColor>(), TArray<FProcMeshTangent>(), true);
}
