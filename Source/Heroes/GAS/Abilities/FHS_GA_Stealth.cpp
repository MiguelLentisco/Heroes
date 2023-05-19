#include "FHS_GA_Stealth.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Effects/FHS_GE_ApplyCooldown.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Sight.h"

// ---------------------------------------------------------------------------------------------------------------------

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Cooldown_Ability_Stealth, TEXT("Cooldown.Ability.Stealth"));
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Name_Ability_Stealth, TEXT("Name.Ability.Stealth"));

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GA_Stealth::UFHS_GA_Stealth()
{
	AbilityTags.AddTag(TAG_Name_Ability_Stealth.GetTag());
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

	// Hide Actor
	AActor* Hero = ActorInfo->AvatarActor.Get();
	Hero->SetActorHiddenInGame(true);
	Hero->Tags.Add(TEXT("OnStealth"));
	
	const float ActualStealthTime = StealthTime.GetValueAtLevel(GetAbilityLevel(Handle, ActorInfo));
	StealthWaitTask = UAbilityTask_WaitDelay::WaitDelay(this, ActualStealthTime);
	StealthWaitTask->OnFinish.AddDynamic(this, &UFHS_GA_Stealth::OnStealthEnded);
	StealthWaitTask->Activate();

	if (!HasAuthority(&ActivationInfo))
	{
		return;
	}

	// Hide from IA and play cue
	UAIPerceptionSystem* AIPerceptionSys = UAIPerceptionSystem::GetCurrent(GetWorld());
	AIPerceptionSys->UnregisterSource(*Hero, UAISense_Sight::StaticClass());
		
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	ASC->ExecuteGameplayCue(GCTag, ASC->MakeEffectContext());
	
} // ActivateAbility

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_Stealth::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Hide from IA and make visible
	AActor* Hero = ActorInfo->AvatarActor.Get();
	Hero->SetActorHiddenInGame(false);
	
	if (HasAuthority(&ActivationInfo))
	{
		UAIPerceptionSystem* AIPerceptionSys = UAIPerceptionSystem::GetCurrent(GetWorld());
		AIPerceptionSys->RegisterSource(*Hero);
	}

	// Apply CD now
	CommitAbility(Handle, ActorInfo, ActivationInfo);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
} // EndAbility

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_Stealth::OnStealthEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	
} // OnStealthEnded

// ---------------------------------------------------------------------------------------------------------------------
