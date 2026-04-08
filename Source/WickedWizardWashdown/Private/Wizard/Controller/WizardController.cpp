// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardController.h"

#include "Wizard/State/WizardState.h"
#include "Wizard/Character/WizardCharacter.h"

AWizardState* AWizardController::GetWizardState() const
{
	return GetPlayerState<AWizardState>();
}

AWizardCharacter* AWizardController::GetWizardCharacter() const
{
	return GetPawn<AWizardCharacter>();
}
