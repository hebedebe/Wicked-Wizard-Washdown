#pragma once

#include "CoreMinimal.h"
#include "SpellFormula.generated.h"

enum class ESpellComponent : uint8;
class ABaseSpell;

USTRUCT(Blueprintable)
struct FSpellFormula
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<ESpellComponent> Components;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ABaseSpell> Spell;
};