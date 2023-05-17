#include "FHS_GE_GainPower.h"

#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GE_GainPower::UFHS_GE_GainPower()
{
	OngoingTagRequirements.IgnoreTags.AddTag(TAG_Status_Dead.GetTag());
	
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = 2.f;
	bExecutePeriodicEffectOnApplication = false;
	
	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UFHS_Attributes_CharacterCore::GetCurrentUltimatePowerAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.ModifierMagnitude = FScalableFloat(5.f);
	
	Modifiers.Add(ModifierInfo);
	
} // UFHS_GE_GainPower

// ---------------------------------------------------------------------------------------------------------------------
