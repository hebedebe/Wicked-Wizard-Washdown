// Fill out your copyright notice in the Description page of Project Settings.


#include "WickedWizardWashdownGameInstance.h"

#include "Wizard/Structs/WizardCustomisationData.h"

void UWickedWizardWashdownGameInstance::SetWizardCustomisationData(const FWizardCustomisationData Data, 
	const int Index)
{
	WizardCustomisationData.Add(Index, Data);
}

FWizardCustomisationData UWickedWizardWashdownGameInstance::GetWizardCustomisationData(const int Index)
{
	if (WizardCustomisationData.Contains(Index))
	{
		return WizardCustomisationData[Index];
	}
	
	/* Return default colours
	UE_LOG(LogTemp, Warning, TEXT("Could not find WizardCustomisationData for index %d, creating default index"), Index);
	SetWizardCustomisationData(FWizardCustomisationData(), Index);
	*/
	
	// Return default colours
	UE_LOG(LogTemp, Warning, TEXT("Could not find WizardCustomisationData for index %d, creating random default index"), Index);
	SetWizardCustomisationData(FWizardCustomisationData::GetRandomWizardCustomisationData(), Index);
	
	
	return GetWizardCustomisationData(Index);
}

void UWickedWizardWashdownGameInstance::ClearWizardCustomisationData(const int Index)
{
	if (Index < 0)
	{
		WizardCustomisationData.Empty();
	} else
	{
		WizardCustomisationData.Remove(Index);
	}
}

int UWickedWizardWashdownGameInstance::GetLastWinningIndex() const
{
	return LastWinningIndex;
}
