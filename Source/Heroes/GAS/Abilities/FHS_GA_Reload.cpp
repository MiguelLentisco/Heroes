#include "FHS_GA_Reload.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_Weapon.h"
#include "Heroes/GAS/Effects/FHS_GE_ReloadAmmo.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GA_Reload::UFHS_GA_Reload()
{
	AbilityTags.AddTag(TAG_Status_AbilityCancelable.GetTag());
	ActivationOwnedTags.AddTag(TAG_Status_Reloading.GetTag());
	CancelAbilitiesWithTag.AddTag(TAG_Status_AbilityCancelable.GetTag());

	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationBlockedTags.AddTag(TAG_Status_Stunned.GetTag());
	ActivationBlockedTags.AddTag(TAG_Status_Dead.GetTag());
	
} // UFHS_GA_Reload

// ---------------------------------------------------------------------------------------------------------------------

bool UFHS_GA_Reload::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo,
                                        const FGameplayTagContainer* SourceTags,
                                        const FGameplayTagContainer* TargetTags,
                                        FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	return ASC->GetNumericAttribute(UFHS_Attributes_Weapon::GetCurrentAmmoAttribute()) < ASC->GetNumericAttribute(
		UFHS_Attributes_Weapon::GetAmmoClipAttribute());
	
} // CanActivateAbility

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_Reload::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

	const float ReloadingTime = ReloadTime.GetValueAtLevel(GetAbilityLevel(Handle, ActorInfo));
	ReloadTask = UAbilityTask_WaitDelay::WaitDelay(this, ReloadingTime);
	ReloadTask->OnFinish.AddDynamic(this, &UFHS_GA_Reload::OnReloadFinished);
	ReloadTask->Activate();

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	ASC->ExecuteGameplayCue(CGTag, ASC->MakeEffectContext());
	
} // ActivateAbility

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_Reload::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ReloadTask != nullptr)
	{
		ReloadTask->OnFinish.RemoveAll(this);
		ReloadTask->MarkAsGarbage();
		ReloadTask = nullptr;
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
} // EndAbility

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_Reload::OnReloadFinished()
{
	UGameplayEffect* GE = UFHS_GE_ReloadAmmo::StaticClass()->GetDefaultObject<UGameplayEffect>();
	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	ASC->ApplyGameplayEffectToSelf(GE, GetAbilityLevel(CurrentSpecHandle, CurrentActorInfo), ASC->MakeEffectContext());
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	
} // OnReloadFinished

// ---------------------------------------------------------------------------------------------------------------------
