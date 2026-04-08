// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArenaGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WICKEDWIZARDWASHDOWN_API AArenaGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
private:
	int PlayersSpawned = 0;
};
