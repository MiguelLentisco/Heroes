#include "FHS_MMC_Cooldown.h"

#include "Heroes/GAS/Abilities/FHS_GameplayAbility.h"

// ---------------------------------------------------------------------------------------------------------------------

float UFHS_MMC_Cooldown::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// From https://github.com/tranek/GASDocumentation#concepts-ge-mmc
	const UFHS_GameplayAbility* Ability = Cast<UFHS_GameplayAbility>(Spec.GetContext().GetAbilityInstance_NotReplicated());

	if (Ability == nullptr)
	{
		return 0.0f;
	}

	return Ability->GetCooldown().GetValueAtLevel(Ability->GetAbilityLevel());
	
} // CalculateBaseMagnitude_Implementation

// ---------------------------------------------------------------------------------------------------------------------

