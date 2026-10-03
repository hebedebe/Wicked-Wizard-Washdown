// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaGameMode.h"

#include <stdexcept>

AActor* AArenaGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	if (!IsValid(Player)) 
		return nullptr;
		// throw std::invalid_argument("Player is invalid"); // <-- dont actually do this i guess since im trying to prevent crashes UGH
	
	FString SpawnTag;
	if (PlayersSpawned++ % 2 == 0)
		SpawnTag = TEXT("Left");
	else
		SpawnTag = TEXT("Right");
	
	return FindPlayerStart(Player, SpawnTag);
}
