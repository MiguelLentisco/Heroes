#include "FHS_UW_HUD.h"

#include "FHS_UW_Attributes.h"

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_HUD::SetupWithGAS_Implementation(UAbilitySystemComponent* ASC)
{
	IFHS_GASListener::Execute_SetupWithGAS(AttributesHUD, ASC);
	
} // SetupWithGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_HUD::CleanFromGAS_Implementation(UAbilitySystemComponent* ASC)
{
	IFHS_GASListener::Execute_CleanFromGAS(AttributesHUD, ASC);
	
} // CleanFromGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

