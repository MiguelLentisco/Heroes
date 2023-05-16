#include "FHS_GA_Jump.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "GameFramework/Character.h"
#include "Heroes/GAS/FHS_GameplayTags.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GA_Jump::UFHS_GA_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ActivationBlockedTags.AddTag(TAG_Status_Stun.GetTag());
	
} // UFHS_GA_Jump

// ---------------------------------------------------------------------------------------------------------------------

bool UFHS_GA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (Character == nullptr || !Character->CanJump())
	{
		return false;
	}
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	
} // CanActivateAbility

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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
	
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (Character == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Character->Jump();
	Character->LandedDelegate.RemoveAll(this);
	Character->LandedDelegate.AddDynamic(this, &UFHS_GA_Jump::OnCharacterLanded);
	
} // ActivateAbility

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_Jump::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	StopJumping();
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	
} // CancelAbility

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	StopJumping();
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	
} // InputReleased

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_Jump::OnCharacterLanded(const FHitResult& Hit)
{
	StopJumping();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	
} // OnCharacterLanded

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_Jump::StopJumping() const
{
	ACharacter* Character = Cast<ACharacter>(GetActorInfo().AvatarActor.Get());
	if (Character == nullptr)
	{
		return;
	}

	Character->StopJumping();
	Character->LandedDelegate.RemoveAll(this);
	
} // StopJumping

// ---------------------------------------------------------------------------------------------------------------------
