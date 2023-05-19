#include "FHS_GE_SuperSpeed.h"

#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GE_SuperSpeed::UFHS_GE_SuperSpeed()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat DurationByCaller;
	DurationByCaller.DataTag = TAG_Data_Duration.GetTag();
	DurationMagnitude = DurationByCaller;

	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UFHS_Attributes_CharacterCore::GetSpeedAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Override;

	FSetByCallerFloat SpeedByCaller;
	SpeedByCaller.DataTag = TAG_Data_Speed.GetTag();
	ModifierInfo.ModifierMagnitude = SpeedByCaller;

	Modifiers.Add(ModifierInfo);
	
} // UFHS_GE_SuperSpeed

// ---------------------------------------------------------------------------------------------------------------------

