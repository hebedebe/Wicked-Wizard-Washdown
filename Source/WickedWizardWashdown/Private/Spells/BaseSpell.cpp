#include "BaseSpell.h"

#include "SpellCastData.h"


ABaseSpell::ABaseSpell()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseSpell::Execute(const FSpellCastData& SourceCastData)
{
	CastData = SourceCastData;
	OnExecute(CastData);
}

AWizardCharacter* ABaseSpell::GetOwningCharacter() const
{
	return CastData.Caster;
}
