// Fill out your copyright notice in the Description page of Project Settings.


#include "MarchingChunk.h"


AMarchingChunk::AMarchingChunk()
{}

void AMarchingChunk::GenerateMesh()
{
	const double StartTime = FPlatformTime::Seconds();
	
	if (SurfaceLevel > 0.f)
	{
		TriangleOrder[0] = 0;
		TriangleOrder[1] = 1;
		TriangleOrder[2] = 2;
	} else
	{
		TriangleOrder[0] = 2;
		TriangleOrder[1] = 1;
		TriangleOrder[2] = 0;
	}

	const int ChunkSize = ChunkFormat.CellsPerChunk;
	float Cube[8];
	
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				for (int i = 0; i < 8; i++)
				{
					Cube[i] = Voxels[GetVoxelIndex(x + VertexOffset[i][0], y + VertexOffset[i][1], z + VertexOffset[i][2])];
				}
				
				March(x, y, z, Cube); // This runs shockingly fast...
			}
		}
	}
	
	const double EndTime = FPlatformTime::Seconds();
	
	const double TimeMs = (EndTime - StartTime) * 1000.0;
	UE_LOG(LogTemp, Warning, TEXT("GenerateMesh took %f ms for a single chunk."), TimeMs);
}

void AMarchingChunk::March(const int X, const int Y, const int Z, const float Cube[8])
{
	int VertexMask = 0;
	FVector EdgeVertex[12];

	for (int i = 0; i < 8; ++i)
	{
		if (Cube[i] <= SurfaceLevel) VertexMask |= 1 << i;
	}
	
	const int EdgeMask = CubeEdgeFlags[VertexMask];
	
	if (EdgeMask == 0) return;
	
	for (int i = 0; i < 12; i++)
	{
		if ((EdgeMask & (1 << i)) != 0)
		{
			const float Offset = Interpolation ? GetInterpolationOffset(Cube[EdgeConnection[i][0]], Cube[EdgeConnection[i][1]]) : .5f;
			
			EdgeVertex[i].X = X + (VertexOffset[EdgeConnection[i][0]][0] + Offset * EdgeDirection[i][0]);
			EdgeVertex[i].Y = Y + (VertexOffset[EdgeConnection[i][0]][1] + Offset * EdgeDirection[i][1]);
			EdgeVertex[i].Z = Z + (VertexOffset[EdgeConnection[i][0]][2] + Offset * EdgeDirection[i][2]);
		}
	}

	for (int i = 0; i < 5; ++i)
	{
		if (TriangleConnectionTable[VertexMask][3 * i] < 0) break;

		const int& CellSize = ChunkFormat.CellSize;
		
		auto V1 = EdgeVertex[TriangleConnectionTable[VertexMask][3 * i]] * CellSize;
		auto V2 = EdgeVertex[TriangleConnectionTable[VertexMask][3 * i + 1]] * CellSize;
		auto V3 = EdgeVertex[TriangleConnectionTable[VertexMask][3 * i + 2]] * CellSize;
		
		auto Normal = FVector::CrossProduct(V2 - V1, V3 - V1);
		Normal.Normalize();
		
		MeshData.Vertices.Add(V1);  
		MeshData.Vertices.Add(V2);
		MeshData.Vertices.Add(V3);
		
		MeshData.Triangles.Add(VertexCount + TriangleOrder[0]);
		MeshData.Triangles.Add(VertexCount + TriangleOrder[1]);
		MeshData.Triangles.Add(VertexCount + TriangleOrder[2]);
		
		MeshData.Normals.Add(Normal);
		MeshData.Normals.Add(Normal);
		MeshData.Normals.Add(Normal);
		
		VertexCount += 3;
	}
}

float AMarchingChunk::GetInterpolationOffset(const float V1, const float V2) const
{
	const float Delta = V2 - V1;
	return Delta == 0.0f ? SurfaceLevel : (SurfaceLevel - V1) / Delta;
}
