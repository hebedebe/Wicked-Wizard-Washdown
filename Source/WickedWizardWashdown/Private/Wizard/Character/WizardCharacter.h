// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WizardCharacter.generated.h"

class AWizardState;
class AWizardController;

UCLASS()
class WICKEDWIZARDWASHDOWN_API AWizardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWizardCharacter();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:
	UFUNCTION(BlueprintCallable)
	AWizardController* GetWizardController() const;
	
	UFUNCTION(BlueprintCallable)
	AWizardState* GetWizardState() const;
	
	UFUNCTION(BlueprintCallable)
	void CastCurrentSpell();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
