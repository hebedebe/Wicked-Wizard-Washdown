// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardController.h"

#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Wizard/State/WizardState.h"
#include "Wizard/Character/WizardCharacter.h"

AWizardController::AWizardController()
{
	bAutoManageActiveCameraTarget = false;
}

AWizardState* AWizardController::GetWizardState() const
{
	return GetPlayerState<AWizardState>();
}

AWizardCharacter* AWizardController::GetWizardCharacter() const
{
	return GetPawn<AWizardCharacter>();
}

void AWizardController::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> Cameras;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraActor::StaticClass(), FName("MainCamera"), Cameras);

	const auto CameraActor = Cameras[0];
	
	if (IsValid(CameraActor))
	{
		ClientSetViewTarget(CameraActor, FViewTargetTransitionParams());
	}
}
