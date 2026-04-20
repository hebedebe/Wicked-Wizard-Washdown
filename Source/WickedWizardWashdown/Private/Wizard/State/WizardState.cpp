// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardState.h"

#include "Wizard/Controller/WizardController.h"
#include "Wizard/Character/WizardCharacter.h"

#include "Engine/DataTable.h"
#include "Spells/Spells/BaseSpell.h"
#include "Spells/Structs/SpellFormula.h"

float AWizardState::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Wizard state took %f damage."), DamageAmount);
	
	SetHealth(GetHealth() - DamageAmount);
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

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

TArray<ESpellComponent> AWizardState::GetSpellBuffer() const
{
	return SpellBuffer;
}

void AWizardState::ClearSpellBuffer()
{
	SpellBuffer.Empty();
}

void AWizardState::BufferSpellComponent(const ESpellComponent Component)
{
	SpellBuffer.Add(Component);
}

float AWizardState::GetMaxHealth() const
{
	return MaxHealth;
}

float AWizardState::GetHealth() const
{
	return Health;
}

void AWizardState::SetMaxHealth(const float Amount)
{
	MaxHealth = Amount;
}

void AWizardState::SetHealth(const float Amount)
{
	Health = FMath::Clamp(Amount, 0, MaxHealth);
}

void AWizardState::AddHealth(const float Amount)
{
	Health = FMath::Clamp(GetHealth() + Amount, 0, MaxHealth);
}
