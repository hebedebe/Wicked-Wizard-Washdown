#pragma once

#include "CoreMinimal.h"
#include "WizardCustomisationData.generated.h"

USTRUCT(BlueprintType)
struct FWizardCustomisationData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Colours")
	FColor HatColour = FColor::Yellow;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Colours")
	FColor HeadColour = FColor::Green;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Colours")
	FColor BodyColour = FColor::Purple;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Colours")
	FColor HandColour = FColor::Red;
};