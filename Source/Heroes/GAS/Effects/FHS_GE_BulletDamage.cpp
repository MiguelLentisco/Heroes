#include "FHS_GE_BulletDamage.h"

#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GE_BulletDamage::UFHS_GE_BulletDamage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	
	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UFHS_Attributes_CharacterCore::GetCurrentHealthAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat ByCaller;
	ByCaller.DataTag = TAG_Data_Damage.GetTag();
	ModifierInfo.ModifierMagnitude = ByCaller;
	
	Modifiers.Add(ModifierInfo);
	
} // UFHS_GE_BulletDamage

// ---------------------------------------------------------------------------------------------------------------------

