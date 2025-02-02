#include "FHS_GE_Orange.h"

#include "GameplayEffectComponents/RemoveOtherGameplayEffectComponent.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GE_Orange::UFHS_GE_Orange()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayTagContainer removeGETagsContainer = FGameplayTagContainer::CreateFromArray(TArray<FGameplayTag>{
		TAG_Status_Stunned, TAG_Status_Poison
	});
	auto effectComponent = CreateDefaultSubobject<URemoveOtherGameplayEffectComponent>(TEXT("RemoveOther"));
	GEComponents.Add(effectComponent);
	effectComponent->RemoveGameplayEffectQueries.Add(FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(removeGETagsContainer));
	
	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UFHS_Attributes_CharacterCore::GetCurrentHealthAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	
	FSetByCallerFloat ByCaller;
	ByCaller.DataTag = TAG_Data_Heal.GetTag();
	ModifierInfo.ModifierMagnitude = ByCaller;

	Modifiers.Add(ModifierInfo);
	
} // UFHS_GE_Orange

// ------------------------------------------------------------------------------
// -																			-
// ------------------------------------------------------------------------------

#if WITH_EDITOR

void UFHS_GE_Orange::PostCDOCompiled(const FPostCDOCompiledContext& Context)
{
	Super::PostCDOCompiled(Context);

	
}

#endif // WITH_EDITOR

// ---------------------------------------------------------------------------------------------------------------------
