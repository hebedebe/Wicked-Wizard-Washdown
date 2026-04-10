#pragma once

#include "Core.h"
#include "ChunkFormat.generated.h"

USTRUCT(BlueprintType)
struct FChunkFormat
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CellSize = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CellsPerChunk = 16; 
};