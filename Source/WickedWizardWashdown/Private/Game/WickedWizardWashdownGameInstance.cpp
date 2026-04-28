// Fill out your copyright notice in the Description page of Project Settings.


#include "WickedWizardWashdownGameInstance.h"

#include "Misc/AssetRegistryInterface.h"
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
	
	UE_LOG(LogTemp, Warning, TEXT("Could not find WizardCustomisationData for index %d, creating default index"), Index);
	SetWizardCustomisationData(FWizardCustomisationData(), Index);
	return GetWizardCustomisationData(Index); //Return default colours if not set.
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
