#include "FHS_UW_Abilities.h"

#include "FHS_UW_Ability.h"

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_Abilities::SetupWithGAS_Implementation(UAbilitySystemComponent* ASC)
{
	if (ASC == nullptr)
	{
		return;
	}

	for (TObjectPtr<UFHS_UW_Ability> AbilityWidget : AbilityWidgets)
	{
		IFHS_GASListener::Execute_SetupWithGAS(AbilityWidget, ASC);
	}
	
} // SetupWithGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_Abilities::CleanFromGAS_Implementation(UAbilitySystemComponent* ASC)
{
	if (ASC == nullptr)
	{
		return;
	}

	for (TObjectPtr<UFHS_UW_Ability> AbilityWidget : AbilityWidgets)
	{
		IFHS_GASListener::Execute_CleanFromGAS(AbilityWidget, ASC);
	}
	
} // CleanFromGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

