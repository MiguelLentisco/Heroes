#include "FHS_GE_ReloadAmmo.h"

#include "Heroes/GAS/Attributes/FHS_Attributes_Weapon.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GE_ReloadAmmo::UFHS_GE_ReloadAmmo()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UFHS_Attributes_Weapon::GetCurrentAmmoAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Override;
	
	FGameplayEffectAttributeCaptureDefinition AttributeDef;
	AttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AttributeDef.bSnapshot = false;
	AttributeDef.AttributeToCapture = UFHS_Attributes_Weapon::GetAmmoClipAttribute();
	FAttributeBasedFloat AmmoClip;
	
	AmmoClip.AttributeCalculationType = EAttributeBasedFloatCalculationType::AttributeMagnitude;
	AmmoClip.BackingAttribute = AttributeDef;
	ModifierInfo.ModifierMagnitude = AmmoClip;
	
	Modifiers.Add(ModifierInfo);
	
} // UFHS_GE_ReloadAmmo

// ---------------------------------------------------------------------------------------------------------------------
