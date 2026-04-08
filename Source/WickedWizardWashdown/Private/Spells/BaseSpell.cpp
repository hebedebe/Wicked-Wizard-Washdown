#include "BaseSpell.h"


ABaseSpell::ABaseSpell()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseSpell::Execute(AWizardCharacter* SourceCharacter)
{
	this->OwningCharacter = SourceCharacter;
	OnExecute();
}

void ABaseSpell::OnExecute()
{
	// Per-spell implementation overrides this function.
}
