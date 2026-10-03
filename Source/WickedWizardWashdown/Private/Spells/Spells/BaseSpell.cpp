#include "BaseSpell.h"

#include "../Structs/SpellCastData.h"
#include "Kismet/GameplayStatics.h"


ABaseSpell::ABaseSpell()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseSpell::Execute(const FSpellCastData& SourceCastData)
{
	UWorld* World = GetWorld();
	if (!IsValid(World)) // waiter one more sanity check please I AM LOSING MY MIND
		return;
	
	CastData = SourceCastData;
	UGameplayStatics::PlaySound2D(GetWorld(), CastSound);
	OnExecute(CastData);
}

AWizardCharacter* ABaseSpell::GetOwningCharacter() const
{
	return CastData.Caster;
}
