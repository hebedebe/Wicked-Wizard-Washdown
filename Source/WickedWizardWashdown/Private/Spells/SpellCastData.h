#pragma once

#include "CoreMinimal.h"
#include "SpellCastData.generated.h"

USTRUCT(BlueprintType)
struct FSpellCastData
{
	GENERATED_BODY()
	
public:
	FVector2D CastDirection;
	FVector CastPosition;
};