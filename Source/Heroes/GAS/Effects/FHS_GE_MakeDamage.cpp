#include "FHS_GE_MakeDamage.h"

#include "GameplayEffectComponents/AssetTagsGameplayEffectComponent.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GE_MakeDamage::UFHS_GE_MakeDamage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	
	FInheritedTagContainer effectDmgContainer;
	effectDmgContainer.AddTag(TAG_Effect_Damage.GetTag());

	auto effectComponent = CreateDefaultSubobject<UAssetTagsGameplayEffectComponent>(TEXT("AssetTags"));
	GEComponents.Add(effectComponent);
	effectComponent->SetAndApplyAssetTagChanges(effectDmgContainer);
	
	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UFHS_Attributes_CharacterCore::GetCurrentHealthAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat ByCaller;
	ByCaller.DataTag = TAG_Data_Damage.GetTag();
	ModifierInfo.ModifierMagnitude = ByCaller;
	
	Modifiers.Add(ModifierInfo);
	
} // UFHS_GE_MakeDamage

// ---------------------------------------------------------------------------------------------------------------------

