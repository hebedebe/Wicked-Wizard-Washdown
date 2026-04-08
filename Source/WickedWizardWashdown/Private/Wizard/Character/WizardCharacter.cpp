// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardCharacter.h"

#include "Spells/BaseSpell.h"
#include "Wizard/Controller/WizardController.h"
#include "Wizard/State/WizardState.h"

AWizardCharacter::AWizardCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWizardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWizardCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

AWizardController* AWizardCharacter::GetWizardController() const
{
	return Cast<AWizardController>(GetLocalViewingPlayerController());
}

AWizardState* AWizardCharacter::GetWizardState() const
{
	return GetPlayerState<AWizardState>();
}

void AWizardCharacter::CastCurrentSpell()
{
	const auto State = GetWizardState();
	const auto SpellClass = State->GetCurrentSpell();
	
	if (IsValid(SpellClass))
	{
		ABaseSpell* Spell = GetWorld()->SpawnActor<ABaseSpell>(SpellClass, GetActorTransform());
		Spell->Execute(this);
	}
}

void AWizardCharacter::BeginPlay()
{
	Super::BeginPlay();
}
