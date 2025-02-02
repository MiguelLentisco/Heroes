#include "FHS_GE_Stun.h"

#include "GameplayEffectComponents/AssetTagsGameplayEffectComponent.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GE_Stun::UFHS_GE_Stun()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FScalableFloat(2.f);
	
	FInheritedTagContainer effectDmgContainer;
	effectDmgContainer.AddTag(TAG_Effect_Damage.GetTag());

	auto effectComponent1 = CreateDefaultSubobject<UAssetTagsGameplayEffectComponent>(TEXT("AssetTags"));
	GEComponents.Add(effectComponent1);
	effectComponent1->SetAndApplyAssetTagChanges(effectDmgContainer);
	
	FInheritedTagContainer stunContainer;
	effectDmgContainer.AddTag(TAG_Status_Stunned.GetTag());
	auto effectComponent2 = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTags"));
	GEComponents.Add(effectComponent2);
	effectComponent2->SetAndApplyTargetTagChanges(stunContainer);
	
} // UFHS_GE_Stun

// ---------------------------------------------------------------------------------------------------------------------

