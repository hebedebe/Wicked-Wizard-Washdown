// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WizardController.generated.h"

class AWizardCharacter;
class AWizardState;

UCLASS()
class WICKEDWIZARDWASHDOWN_API AWizardController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	AWizardState* GetWizardState() const;
	
	UFUNCTION(BlueprintCallable)
	AWizardCharacter* GetWizardCharacter() const;
};
