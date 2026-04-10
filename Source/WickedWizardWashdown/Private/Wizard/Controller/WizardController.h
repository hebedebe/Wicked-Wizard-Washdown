// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WizardController.generated.h"

class UInputMappingContext;
class AWizardCharacter;
class AWizardState;

UCLASS()
class WICKEDWIZARDWASHDOWN_API AWizardController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AWizardController();
	
public:
	UFUNCTION(BlueprintCallable)
	AWizardState* GetWizardState() const;
	
	UFUNCTION(BlueprintCallable)
	AWizardCharacter* GetWizardCharacter() const;
	
public:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;
	
protected:
	void BeginPlay() override;
};
