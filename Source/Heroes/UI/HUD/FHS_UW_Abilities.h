#pragma once

#include "FHS_GASListener.h"
#include "Blueprint/UserWidget.h"
#include "FHS_UW_Abilities.generated.h"

class UFHS_UW_Ability;

UCLASS(Abstract)
class UFHS_UW_Abilities : public UUserWidget, public IFHS_GASListener
{
	GENERATED_BODY()

public:
	virtual void SetupWithGAS_Implementation(UAbilitySystemComponent* ASC) override;
	virtual void CleanFromGAS_Implementation(UAbilitySystemComponent* ASC) override;
	
protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UFHS_UW_Ability>> AbilityWidgets;
	
}; // UFHS_UW_Abilities
