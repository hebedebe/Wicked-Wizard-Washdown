#include "ChunkBase.h"

#include "External/FastNoiseLite/FastNoiseLite.h"


AChunkBase::AChunkBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	Noise = new FastNoiseLite();
	
	Noise->SetFrequency(Frequency);
	Noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	Noise->SetFractalType(FastNoiseLite::FractalType_FBm);
	
	// Mesh Settings
	Mesh->SetCastShadow(false);
	
	// Set Mesh as root
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AChunkBase::BeginPlay()
{
	Super::BeginPlay();

	GenerateHeightMap();
	
	GenerateMesh();
	
	UE_LOG(LogTemp, Warning, TEXT("Vertex Count : %d"), VertexCount);
	
	ApplyMesh();
}

void AChunkBase::GenerateHeightMap()
{
}

void AChunkBase::GenerateMesh()
{
}

void AChunkBase::ApplyMesh() const
{
	Mesh->CreateMeshSection(0, MeshData.Vertices, MeshData.Triangles, MeshData.Normals, MeshData.UV0, 
		TArray<FColor>(), TArray<FProcMeshTangent>(), true);
}
