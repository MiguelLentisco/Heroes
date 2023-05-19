#include "FHS_GA_Stealth.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Effects/FHS_GE_ApplyCooldown.h"

// ---------------------------------------------------------------------------------------------------------------------

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Cooldown_Ability_Stealth, TEXT("Cooldown.Ability.Stealth"));

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GA_Stealth::UFHS_GA_Stealth()
{
	AbilityTags.AddTag(TAG_Status_AbilityCancelable.GetTag());

	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CancelAbilitiesWithTag.AddTag(TAG_Status_AbilityCancelable.GetTag());

	ActivationBlockedTags.AddTag(TAG_Status_Stunned.GetTag());
	ActivationBlockedTags.AddTag(TAG_Status_Dead.GetTag());

	CooldownGameplayEffectClass = UFHS_GE_ApplyCooldown::StaticClass();
	bUseScalarCooldown = true;
	CooldownScalar = 15.f;
	CooldownTags.AddTag(TAG_Cooldown_Ability_Stealth.GetTag());
	
} // UFHS_GA_Stealth

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_Stealth::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ActorInfo->AvatarActor->SetActorHiddenInGame(true);
	const float ActualStealthTime = StealthTime.GetValueAtLevel(GetAbilityLevel(Handle, ActorInfo));
	StealthWaitTask = UAbilityTask_WaitDelay::WaitDelay(this, ActualStealthTime);
	StealthWaitTask->OnFinish.AddDynamic(this, &UFHS_GA_Stealth::OnStealthEnded);
	StealthWaitTask->Activate();

	if (HasAuthority(&ActivationInfo))
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		ASC->ExecuteGameplayCue(GCTag, ASC->MakeEffectContext());
	}
	
} // ActivateAbility

// ---------------------------------------------------------------------------------------------------------------------


void UFHS_GA_Stealth::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CurrentActorInfo->AvatarActor->SetActorHiddenInGame(false);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
} // EndAbility

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_Stealth::OnStealthEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	
} // OnStealthEnded

// ---------------------------------------------------------------------------------------------------------------------
