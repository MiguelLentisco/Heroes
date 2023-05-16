#include "FHS_UW_HUD.h"

#include "FHS_UW_Attributes.h"
#include "FHS_UW_Abilities.h"
#include "FHS_UW_Weapon.h"

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_HUD::OnHeroInputChangedInput(UAbilitySystemComponent* ASC, bool bSet)
{
	if (ASC == nullptr)
	{
		return;
	}

	if (bSet)
	{
		IFHS_GASListener::Execute_SetupWithGAS(AttributesHUD, ASC);
		IFHS_GASListener::Execute_SetupWithGAS(AbilitiesHUD, ASC);
	}
	else
	{
		IFHS_GASListener::Execute_CleanFromGAS(AttributesHUD, ASC);
		IFHS_GASListener::Execute_CleanFromGAS(AbilitiesHUD, ASC);
	}
	
} // OnHeroInputChangedInput

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_HUD::OnHeroWeaponInputChangedInput(UAbilitySystemComponent* ASC, bool bSet)
{
	if (ASC == nullptr)
	{
		return;
	}

	if (bSet)
	{
		IFHS_GASListener::Execute_SetupWithGAS(WeaponHUD, ASC);
	}
	else
	{
		IFHS_GASListener::Execute_CleanFromGAS(WeaponHUD, ASC);
	}
	
} // OnHeroWeaponInputChangedInput

// ---------------------------------------------------------------------------------------------------------------------

