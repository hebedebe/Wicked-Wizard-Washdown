// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSpell.generated.h"

class AWizardCharacter;

UCLASS()
class WICKEDWIZARDWASHDOWN_API ABaseSpell : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseSpell();
	
public:
	void Execute(AWizardCharacter* SourceCharacter);
	
public:
	virtual void OnExecute();
	
public:
	UPROPERTY(BlueprintReadWrite)
	AWizardCharacter* OwningCharacter;
};
