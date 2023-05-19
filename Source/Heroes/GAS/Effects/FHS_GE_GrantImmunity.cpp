#include "FHS_GE_GrantImmunity.h"

#include "Heroes/GAS/FHS_GameplayTags.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GE_GrantImmunity::UFHS_GE_GrantImmunity()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	InheritableOwnedTagsContainer.AddTag(TAG_Status_Immunity.GetTag());
	GrantedApplicationImmunityTags.RequireTags = FGameplayTagContainer{TAG_Effect_Damage.GetTag()};
	
} // UFHS_GE_GrantImmunity

// ---------------------------------------------------------------------------------------------------------------------
