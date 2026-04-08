#pragma once

#include "Core.h"
#include "ChunkFormat.generated.h"

USTRUCT(BlueprintType)
struct FChunkFormat
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	int CellSize = 10;
	
	UPROPERTY(BlueprintReadWrite)
	int CellsPerChunk = 16; 
};