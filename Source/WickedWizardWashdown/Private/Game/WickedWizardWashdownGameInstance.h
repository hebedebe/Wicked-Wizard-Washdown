// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Wizard/Structs/WizardCustomisationData.h"
#include "WickedWizardWashdownGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WICKEDWIZARDWASHDOWN_API UWickedWizardWashdownGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWizardCustomisationData(FWizardCustomisationData Data, int Index);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FWizardCustomisationData GetWizardCustomisationData(int Index);
	
	UFUNCTION(BlueprintCallable)
	void ClearWizardCustomisationData(int Index=-1); // Index of -1 will clear all data, otherwise clear only the specified index
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetLastWinningIndex() const;
	
public:
	UPROPERTY()
	int LastWinningIndex = -1;
	
protected:
	UPROPERTY()
	TMap<int, FWizardCustomisationData> WizardCustomisationData;
};
