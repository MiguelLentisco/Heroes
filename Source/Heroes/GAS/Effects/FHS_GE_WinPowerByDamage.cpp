#include "FHS_GE_WinPowerByDamage.h"

#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GE_WinPowerByDamage::UFHS_GE_WinPowerByDamage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UFHS_Attributes_CharacterCore::GetCurrentUltimatePowerAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.ModifierMagnitude = FScalableFloat(8.f);

	Modifiers.Add(ModifierInfo);
	
} // UFHS_GE_WinPowerByDamage

// ---------------------------------------------------------------------------------------------------------------------
