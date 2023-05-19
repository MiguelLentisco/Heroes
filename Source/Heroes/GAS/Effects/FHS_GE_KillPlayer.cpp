#include "FHS_GE_KillPlayer.h"

#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GE_KillPlayer::UFHS_GE_KillPlayer()
{
	InheritableGameplayEffectTags.AddTag(TAG_Status_Dead.GetTag());
	InheritableOwnedTagsContainer.AddTag(TAG_Status_Dead.GetTag());
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	
} // UFHS_GE_KillPlayer

// ---------------------------------------------------------------------------------------------------------------------

