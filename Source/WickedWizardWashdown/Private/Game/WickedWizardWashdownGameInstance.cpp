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
	
	return FWizardCustomisationData(); //Return default colours if not set.
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
