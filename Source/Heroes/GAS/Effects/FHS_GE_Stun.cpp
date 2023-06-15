#include "FHS_GE_Stun.h"

#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GE_Stun::UFHS_GE_Stun()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FScalableFloat(2.f);

	InheritableGameplayEffectTags.AddTag(TAG_Effect_Damage.GetTag());
	InheritableOwnedTagsContainer.AddTag(TAG_Status_Stunned.GetTag());
	
} // UFHS_GE_Stun

// ---------------------------------------------------------------------------------------------------------------------

