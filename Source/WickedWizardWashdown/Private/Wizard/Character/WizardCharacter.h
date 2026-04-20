// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "InputMappingContext.h"
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
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
public:
	UFUNCTION(BlueprintCallable)
	AWizardController* GetWizardController() const;
	
	UFUNCTION(BlueprintCallable)
	AWizardState* GetWizardState() const;
	
	UFUNCTION(BlueprintCallable)
	void CastCurrentSpell(FVector2D Direction);
	
public:
	UFUNCTION()
	void CastHandler(const FInputActionValue& Value);
	
	UFUNCTION()
	void MoveHandler(const FInputActionValue& Value);
	
	UFUNCTION()
	void JumpHandler(const FInputActionValue& Value);
	
	UFUNCTION()
	void StopJumpHandler(const FInputActionValue& Value);
	
	UFUNCTION()
	void ComposeSpellLeftHandler(const FInputActionValue& Value);
	
	UFUNCTION()
	void ComposeSpellUpHandler(const FInputActionValue& Value);
	
	UFUNCTION()
	void ComposeSpellRightHandler(const FInputActionValue& Value);
	
public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CastAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ComposeSpellLeftAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ComposeSpellUpAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ComposeSpellRightAction;
	
};
