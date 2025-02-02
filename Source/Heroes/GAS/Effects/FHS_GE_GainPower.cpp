#include "FHS_GE_GainPower.h"

#include "GameplayEffectComponents/TargetTagRequirementsGameplayEffectComponent.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GE_GainPower::UFHS_GE_GainPower()
{
	FInheritedTagContainer effectDmgContainer;
	effectDmgContainer.AddTag(TAG_Effect_Damage.GetTag());

	auto effectComponent = CreateDefaultSubobject<UTargetTagRequirementsGameplayEffectComponent>(TEXT("TargetTagRequirements"));
	GEComponents.Add(effectComponent);
	effectComponent->OngoingTagRequirements.IgnoreTags.AddTag(TAG_Status_Dead.GetTag());
	
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = 2.f;
	bExecutePeriodicEffectOnApplication = false;
	
	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UFHS_Attributes_CharacterCore::GetCurrentUltimatePowerAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.ModifierMagnitude = FScalableFloat(3.f);
	
	Modifiers.Add(ModifierInfo);
	
} // UFHS_GE_GainPower

// ---------------------------------------------------------------------------------------------------------------------
