#include "BaseSpell.h"

#include "../Structs/SpellCastData.h"
#include "Kismet/GameplayStatics.h"


ABaseSpell::ABaseSpell()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseSpell::Execute(const FSpellCastData& SourceCastData)
{
	CastData = SourceCastData;
	UGameplayStatics::PlaySound2D(GetWorld(), CastSound);
	OnExecute(CastData);
}

AWizardCharacter* ABaseSpell::GetOwningCharacter() const
{
	return CastData.Caster;
}
