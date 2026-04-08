#pragma once

#include "CoreMinimal.h"
#include "SpellCastData.generated.h"

USTRUCT(BlueprintType)
struct FSpellCastData
{
	GENERATED_BODY()
	
public:
	double CastAngle;
	FVector CastPosition;
};