#pragma once

#include "FHS_GASListener.h"
#include "Blueprint/UserWidget.h"
#include "FHS_UW_HUD.generated.h"

class UFHS_UW_Attributes;
class UFHS_UW_Abilities;
class UFHS_UW_Weapon;

UCLASS(Abstract)
class UFHS_UW_HUD : public UUserWidget, public IFHS_GASListener
{
	GENERATED_BODY()
	
public:
	virtual void SetupWithGAS_Implementation(UAbilitySystemComponent* ASC,
	                                         const TArray<UAbilitySystemComponent*>& WeaponASCs) override;
	virtual void CleanFromGAS_Implementation(UAbilitySystemComponent* ASC,
	                                         const TArray<UAbilitySystemComponent*>& WeaponASCs) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UFHS_UW_Attributes> AttributesHUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UFHS_UW_Abilities> AbilitiesHUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UFHS_UW_Weapon> WeaponHUD;
	
}; // UFHS_UW_HUD
