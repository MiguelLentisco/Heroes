#include "FHS_UW_HUD.h"

#include "FHS_UW_Attributes.h"
#include "FHS_UW_Abilities.h"
#include "FHS_UW_Weapon.h"

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_HUD::SetupWithGAS_Implementation(UAbilitySystemComponent* ASC,
                                              const TArray<UAbilitySystemComponent*>& WeaponASCs)
{
	if (ASC == nullptr)
	{
		return;
	}
	
	IFHS_GASListener::Execute_SetupWithGAS(AttributesHUD, ASC, WeaponASCs);
	IFHS_GASListener::Execute_SetupWithGAS(AbilitiesHUD, ASC, WeaponASCs);
	IFHS_GASListener::Execute_SetupWithGAS(WeaponHUD, ASC, WeaponASCs);
	
} // SetupWithGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_HUD::CleanFromGAS_Implementation(UAbilitySystemComponent* ASC,
                                              const TArray<UAbilitySystemComponent*>& WeaponASCs)
{
	if (ASC == nullptr)
	{
		return;
	}
	
	IFHS_GASListener::Execute_CleanFromGAS(AttributesHUD, ASC, WeaponASCs);
	IFHS_GASListener::Execute_CleanFromGAS(AbilitiesHUD, ASC, WeaponASCs);
	IFHS_GASListener::Execute_CleanFromGAS(WeaponHUD, ASC, WeaponASCs);
	
} // CleanFromGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

