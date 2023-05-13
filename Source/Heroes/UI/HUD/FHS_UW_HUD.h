#pragma once

#include "FHS_GASListener.h"
#include "Blueprint/UserWidget.h"
#include "FHS_UW_HUD.generated.h"

class UFHS_UW_Attributes;

UCLASS(Abstract)
class UFHS_UW_HUD : public UUserWidget, public IFHS_GASListener
{
	GENERATED_BODY()
	
public:
	virtual void SetupWithGAS_Implementation(UAbilitySystemComponent* ASC) override;
	virtual void CleanFromGAS_Implementation(UAbilitySystemComponent* ASC) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UFHS_UW_Attributes> AttributesHUD;
	
}; // UFHS_UW_HUD
