#include "FHS_GE_FullHealth.h"

#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GE_FullHealth::UFHS_GE_FullHealth()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UFHS_Attributes_CharacterCore::GetCurrentHealthAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Override;
	
	FGameplayEffectAttributeCaptureDefinition AttributeDef;
	AttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AttributeDef.bSnapshot = false;
	AttributeDef.AttributeToCapture = UFHS_Attributes_CharacterCore::GetMaxHealthAttribute();
	FAttributeBasedFloat AmmoClip;
	
	AmmoClip.AttributeCalculationType = EAttributeBasedFloatCalculationType::AttributeMagnitude;
	AmmoClip.BackingAttribute = AttributeDef;
	ModifierInfo.ModifierMagnitude = AmmoClip;

	Modifiers.Add(ModifierInfo);
	
} // UFHS_GE_FullHealth

// ---------------------------------------------------------------------------------------------------------------------

