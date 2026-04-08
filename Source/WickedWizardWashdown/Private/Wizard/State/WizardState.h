// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Engine/DataTable.h"
#include "WizardState.generated.h"

enum class ESpellComponent : uint8;
class ABaseSpell;
class AWizardCharacter;
class AWizardController;
/**
 * 
 */
UCLASS()
class WICKEDWIZARDWASHDOWN_API AWizardState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	bool HasValidSpell();
	
	UFUNCTION(BlueprintCallable)
	TSubclassOf<ABaseSpell> GetCurrentSpell();
	
	UFUNCTION(BlueprintCallable)
	AWizardController* GetWizardController() const;
	
	UFUNCTION(BlueprintCallable)
	AWizardCharacter* GetWizardCharacter() const;
	
	UFUNCTION(BlueprintCallable)
	void ClearSpellBuffer();
	
	UFUNCTION(BlueprintCallable)
	void BufferSpellComponent(ESpellComponent Component);
	
public:
	TArray<ESpellComponent> SpellBuffer; // The spell inputs before being converted to a spell

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Registries")
	UDataTable* SpellRegistry;
};
