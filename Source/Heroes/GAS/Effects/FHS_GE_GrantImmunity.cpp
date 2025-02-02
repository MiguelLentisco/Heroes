#include "FHS_GE_GrantImmunity.h"

#include "GameplayEffectComponents/AssetTagsGameplayEffectComponent.h"
#include "GameplayEffectComponents/ImmunityGameplayEffectComponent.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Heroes/GAS/FHS_GameplayTags.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GE_GrantImmunity::UFHS_GE_GrantImmunity()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	FInheritedTagContainer immunityTagContainer;
	immunityTagContainer.AddTag(TAG_Status_Immunity.GetTag());
	auto effectComponent1 = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTags"));
	GEComponents.Add(effectComponent1);
	effectComponent1->SetAndApplyTargetTagChanges(immunityTagContainer);

	FGameplayEffectQuery damageImmuneQuery;
	FGameplayTagQueryExpression queryExpression = FGameplayTagQueryExpression().AllTagsMatch().AddTags(
		TAG_Effect_Damage.GetTag().GetSingleTagContainer());
	damageImmuneQuery.SourceTagQuery.Build(queryExpression,TEXT("GrantedApplicationImmunityTags"));
	auto effectComponent2 = CreateDefaultSubobject<UImmunityGameplayEffectComponent>(TEXT("Immunity"));
	GEComponents.Add(effectComponent2);
	effectComponent2->ImmunityQueries.Add(damageImmuneQuery);
	
} // UFHS_GE_GrantImmunity

// ---------------------------------------------------------------------------------------------------------------------
