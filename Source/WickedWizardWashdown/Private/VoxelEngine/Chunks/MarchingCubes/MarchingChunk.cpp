// Fill out your copyright notice in the Description page of Project Settings.


#include "MarchingChunk.h"


AMarchingChunk::AMarchingChunk()
{}

void AMarchingChunk::GenerateMesh()
{
	Super::GenerateMesh();
	
	if (bSmoothNormals)
		VertexIndexMap.Empty();
	
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
					const int* Offset = VertexOffset[i];
					Cube[i] = Voxels[GetVoxelIndex(x + Offset[0], y + Offset[1], z + Offset[2])];
				}
				
				March(x, y, z, Cube);
			}
		}
	}

	if (bSmoothNormals)
		FinalizeNormals();
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
		if ((EdgeMask & (1 << i)) != 0) // Shortcut for powers of 2 (2^i)
		{
			
			FVector& TargetVertex = EdgeVertex[i];
			const int& TargetConnection = EdgeConnection[i][0];
			const float* TargetDirection = EdgeDirection[i];
			
			const float Offset = bInterpolation ? GetInterpolationOffset(Cube[EdgeConnection[i][0]], Cube[EdgeConnection[i][1]]) : .5f;
			
			TargetVertex.X = X + (VertexOffset[TargetConnection][0] + Offset * TargetDirection[0]);
			TargetVertex.Y = Y + (VertexOffset[TargetConnection][1] + Offset * TargetDirection[1]);
			TargetVertex.Z = Z + (VertexOffset[TargetConnection][2] + Offset * TargetDirection[2]);
		}
	}

	const int& CellSize = ChunkFormat.CellSize;
	const int* TriRow = TriangleConnectionTable[VertexMask];
	
	for (int i = 0; i < 5; ++i)
	{
		const int Index = 3 * i;
		
		if (TriRow[Index] < 0) break;
		
		auto V1 = EdgeVertex[TriRow[Index]] * CellSize;
		auto V2 = EdgeVertex[TriRow[Index + 1]] * CellSize;
		auto V3 = EdgeVertex[TriRow[Index + 2]] * CellSize;
		
		if (bSmoothNormals)
		{
			const FVector FaceNormal = FVector::CrossProduct(V2 - V1, V3 - V1);

			const FVector Positions[3] = { V1, V2, V3 };
			int32 Indices[3];

			for (int j = 0; j < 3; j++)
			{
				const FIntVector Key = QuantizeVertex(Positions[j]);
				const int32* Existing = VertexIndexMap.Find(Key);

				if (Existing)
				{
					Indices[j] = *Existing;
					MeshData.Normals[Indices[j]] += FaceNormal; // accumulate
				}
				else
				{
					Indices[j] = MeshData.Vertices.Num();
					VertexIndexMap.Add(Key, Indices[j]);
					MeshData.Vertices.Add(Positions[j]);
					MeshData.Normals.Add(FaceNormal);
				}
			}

			MeshData.Triangles.Add(Indices[TriangleOrder[0]]);
			MeshData.Triangles.Add(Indices[TriangleOrder[1]]);
			MeshData.Triangles.Add(Indices[TriangleOrder[2]]);
		} else
		{
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
}

float AMarchingChunk::GetInterpolationOffset(const float V1, const float V2) const
{
	const float Delta = V2 - V1;
	return Delta == 0.0f ? SurfaceLevel : (SurfaceLevel - V1) / Delta;
}

void AMarchingChunk::FinalizeNormals()
{
	for (FVector& N : MeshData.Normals)
		N.Normalize();
}

FIntVector AMarchingChunk::QuantizeVertex(const FVector& Vector, const int32 Scale)
{
	return FIntVector (
		FMath::RoundToInt(Vector.X * Scale),
		FMath::RoundToInt(Vector.Y * Scale),
		FMath::RoundToInt(Vector.Z * Scale)
	);
}
