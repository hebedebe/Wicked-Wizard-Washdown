// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellCastData.h"
#include "GameFramework/Actor.h"
#include "BaseSpell.generated.h"

UCLASS()
class WICKEDWIZARDWASHDOWN_API ABaseSpell : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseSpell();
	
public:
	void Execute(const FSpellCastData& SourceCastData);
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnExecute(const FSpellCastData& SourceCastData);
	
public:
	UPROPERTY(BlueprintReadWrite)
	FSpellCastData CastData;
};
