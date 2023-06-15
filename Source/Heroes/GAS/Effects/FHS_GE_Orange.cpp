#include "FHS_GE_Orange.h"

#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GE_Orange::UFHS_GE_Orange()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	RemoveGameplayEffectsWithTags.AddTag(TAG_Status_Stunned);
	RemoveGameplayEffectsWithTags.AddTag(TAG_Status_Poison);

	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UFHS_Attributes_CharacterCore::GetCurrentHealthAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	
	FSetByCallerFloat ByCaller;
	ByCaller.DataTag = TAG_Data_Heal.GetTag();
	ModifierInfo.ModifierMagnitude = ByCaller;

	Modifiers.Add(ModifierInfo);
	
} // UFHS_GE_Orange

// ---------------------------------------------------------------------------------------------------------------------
