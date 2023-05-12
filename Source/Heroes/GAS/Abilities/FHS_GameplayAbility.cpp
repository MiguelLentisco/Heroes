#include "FHS_GameplayAbility.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GameplayAbility::UFHS_GameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
} // UFHS_GameplayAbility

// ---------------------------------------------------------------------------------------------------------------------

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
	// From https://github.com/tranek/GASDocumentation#concepts-ge-cooldown
	const UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (CooldownGE == nullptr)
	{
		return;
	}

	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
	SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTags);
	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	
} // ApplyCooldown

// ---------------------------------------------------------------------------------------------------------------------

