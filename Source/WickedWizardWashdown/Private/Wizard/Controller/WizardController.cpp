// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardController.h"

#include "EnhancedInputModule.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Wizard/State/WizardState.h"
#include "Wizard/Character/WizardCharacter.h"

class UEnhancedInputLocalPlayerSubsystem;

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
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		if (const ULocalPlayer* LocalPlayer = GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				if (!IsValid(DefaultMappingContext))
				{
					UE_LOG(LogEnhancedInput, Warning, TEXT("The DefaultMappingContext variable has not been set."));
				}
				
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			} else
			{
				UE_LOG(LogActor, Error, TEXT("AWizardController could not access input subsystem"));
			}
		} else
		{
			UE_LOG(LogActor, Error, TEXT("AWizardController could not access local player"));
		}
	}, 0.01, false);
}
