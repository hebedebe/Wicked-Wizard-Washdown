// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardState.h"

#include "Wizard/Controller/WizardController.h"
#include "Wizard/Character/WizardCharacter.h"

#include "Engine/DataTable.h"
#include "Spells/BaseSpell.h"
#include "Spells/SpellFormula.h"

bool AWizardState::HasValidSpell()
{
	return IsValid(GetCurrentSpell());
}

TSubclassOf<ABaseSpell> AWizardState::GetCurrentSpell()
{
	TArray<FSpellFormula*> SpellFormulas;
	SpellRegistry->GetAllRows(TEXT("Retrieving spell registry rows"), SpellFormulas);
	
	for (const auto SpellFormula : SpellFormulas)
	{
		if (SpellFormula->Components.Num() != SpellBuffer.Num()) continue;
		bool bFoundSpell = true;
		for (int i = 0; i < SpellBuffer.Num(); i++)
		{
			if (SpellFormula->Components[i] != SpellBuffer[i])
			{
				bFoundSpell = false;
				break;
			}
		}
		if (bFoundSpell) return SpellFormula->Spell;
	}
	
	return nullptr;
}

AWizardController* AWizardState::GetWizardController() const
{
	return Cast<AWizardController>(GetPlayerController());
}

AWizardCharacter* AWizardState::GetWizardCharacter() const
{
	return GetPawn<AWizardCharacter>();
}

void AWizardState::ClearSpellBuffer()
{
	SpellBuffer.Empty();
}

void AWizardState::BufferSpellComponent(const ESpellComponent Component)
{
	SpellBuffer.Add(Component);
}
