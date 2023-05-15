#include "FHS_GE_ApplyCooldown.h"

#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Abilities/FHS_GameplayAbility.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GE_ApplyCooldown::UFHS_GE_ApplyCooldown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat ByCaller;
	ByCaller.DataTag = TAG_Data_Cooldown.GetTag();
	DurationMagnitude = ByCaller;
	
} // UFHS_GE_ApplyCooldown

// ---------------------------------------------------------------------------------------------------------------------

