// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaGameMode.h"

AActor* AArenaGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	FString SpawnTag;
	if (PlayersSpawned++ % 2 == 0)
		SpawnTag = TEXT("Left");
	else
		SpawnTag = TEXT("Right");
	
	return FindPlayerStart(Player, SpawnTag);
}
