#include "FHS_GE_MakeDamage.h"

#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GE_MakeDamage::UFHS_GE_MakeDamage()
{
	InheritableGameplayEffectTags.AddTag(TAG_Effect_Damage.GetTag());
	DurationPolicy = EGameplayEffectDurationType::Instant;
	
	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UFHS_Attributes_CharacterCore::GetCurrentHealthAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat ByCaller;
	ByCaller.DataTag = TAG_Data_Damage.GetTag();
	ModifierInfo.ModifierMagnitude = ByCaller;
	
	Modifiers.Add(ModifierInfo);
	
} // UFHS_GE_MakeDamage

// ---------------------------------------------------------------------------------------------------------------------

