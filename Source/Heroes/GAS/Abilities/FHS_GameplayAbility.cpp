#include "FHS_GameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Heroes/GAS/FHS_GameplayTags.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GameplayAbility::UFHS_GameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
} // UFHS_GameplayAbility

// ---------------------------------------------------------------------------------------------------------------------

#pragma region Costs

UGameplayEffect* UFHS_GameplayAbility::GetCostGameplayEffect() const
{
	if (AttributeCosts.IsEmpty())
	{
		return Super::GetCostGameplayEffect();
	}

	UGameplayEffect* GECosts = NewObject<UGameplayEffect>();
	GECosts->DurationPolicy = EGameplayEffectDurationType::Instant;

	for (const TPair<FGameplayAttribute, FScalableFloat>& AttributeCost : AttributeCosts)
	{
		FGameplayModifierInfo ModifierInfo;
		ModifierInfo.ModifierOp = EGameplayModOp::Additive;
		ModifierInfo.Attribute = AttributeCost.Key;
		ModifierInfo.ModifierMagnitude = AttributeCost.Value;
		
		GECosts->Modifiers.Add(ModifierInfo);
	}

	return GECosts;
	
} // GetCostGameplayEffect

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	// Hack in order to use runtime GE for applying effect costs
	const UGameplayEffect* CostGE = GetCostGameplayEffect();
	if (CostGE == nullptr || !HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		return;
	}

	const int32 AbilityLevel = GetAbilityLevel(Handle, ActorInfo);
	FGameplayEffectSpec* NewSpec = new FGameplayEffectSpec(CostGE, MakeEffectContext(Handle, ActorInfo), AbilityLevel);
	const FGameplayEffectSpecHandle SpecHandle = FGameplayEffectSpecHandle(NewSpec);

	if (!SpecHandle.IsValid())
	{
		return;
	}
	
	FGameplayAbilitySpec* AbilitySpec = ActorInfo->AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
	ApplyAbilityTagsToGameplayEffectSpec(*SpecHandle.Data.Get(), AbilitySpec);

	// Copy over set by caller magnitudes
	if (AbilitySpec != nullptr)
	{
		SpecHandle.Data->SetByCallerTagMagnitudes = AbilitySpec->SetByCallerTagMagnitudes;
	}
	
	SpecHandle.Data->StackCount = AbilityLevel;
	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	
} // ApplyCost

#pragma endregion // Costs

// ---------------------------------------------------------------------------------------------------------------------

#pragma region Cooldown

const FGameplayTagContainer* UFHS_GameplayAbility::GetCooldownTags() const
{
	// From https://github.com/tranek/GASDocumentation#concepts-ge-cooldown
	FGameplayTagContainer* MutableTags = const_cast<FGameplayTagContainer*>(&TempCooldownTags);
	MutableTags->Reset();
	
	if (const FGameplayTagContainer* ParentTags = Super::GetCooldownTags())
	{
		MutableTags->AppendTags(*ParentTags);
	}
	MutableTags->AppendTags(CooldownTags);
	
	return MutableTags;
	
} // GetCooldownTags

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	const UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (CooldownGE == nullptr)
	{
		return;
	}

	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
	SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTags);

	float Cooldown;
	if (bUseScalarCooldown)
	{
		Cooldown = CooldownScalar.GetValueAtLevel(GetAbilityLevel());
	}
	else
	{
		Cooldown = ActorInfo->AbilitySystemComponent.Get()->GetNumericAttribute(CooldownAttribute);
	}
	
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(TAG_Data_Cooldown, Cooldown);
	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	
} // ApplyCooldown

#pragma endregion // Cooldown

// ---------------------------------------------------------------------------------------------------------------------

