#include "FHS_GE_KillPlayer.h"

#include "GameplayEffectComponents/AssetTagsGameplayEffectComponent.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Heroes/GAS/FHS_GameplayTags.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GE_KillPlayer::UFHS_GE_KillPlayer()
{
	FInheritedTagContainer deadTagContainer;
	deadTagContainer.AddTag(TAG_Status_Dead.GetTag());

	auto effectComponent1 = CreateDefaultSubobject<UAssetTagsGameplayEffectComponent>(TEXT("AssetTags"));
	GEComponents.Add(effectComponent1);
	effectComponent1->SetAndApplyAssetTagChanges(deadTagContainer);

	auto effectComponent2 = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTags"));
	GEComponents.Add(effectComponent2);
	effectComponent2->SetAndApplyTargetTagChanges(deadTagContainer);
	
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	
} // UFHS_GE_KillPlayer

// ---------------------------------------------------------------------------------------------------------------------

