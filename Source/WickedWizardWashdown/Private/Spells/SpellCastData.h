#pragma once

#include "CoreMinimal.h"
#include "SpellCastData.generated.h"

class AWizardCharacter;

USTRUCT(BlueprintType, Blueprintable)
struct FSpellCastData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	FVector2D CastDirection = FVector2D::ZeroVector;
	
	UPROPERTY(BlueprintReadWrite)
	FVector CastPosition = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadWrite)
	AWizardCharacter* Caster = nullptr;
};