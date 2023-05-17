#include "FHS_GA_Dash.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Heroes/GAS/FHS_AbilitySystemComponent.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Effects/FHS_GE_ApplyCooldown.h"
#include "Heroes/GAS/Tasks/FHS_AT_Dash.h"

// ---------------------------------------------------------------------------------------------------------------------

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Name_Ability_Dash, TEXT("Name.Ability.Dash"));
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Cooldown_Ability_Dash, TEXT("Cooldown.Ability.Dash"));

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GA_Dash::UFHS_GA_Dash()
{
	AbilityTags.AddTag(TAG_Name_Ability_Dash.GetTag());
	
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ActivationBlockedTags.AddTag(TAG_Status_Stun.GetTag());
	CancelAbilitiesWithTag.AddTag(TAG_Name_Ability_Reload.GetTag());
	
	CooldownGameplayEffectClass = UFHS_GE_ApplyCooldown::StaticClass();
	bUseScalarCooldown = true;
	CooldownScalar = 5.f;
	CooldownTags.AddTag(TAG_Cooldown_Ability_Dash.GetTag());
	
} // UFHS_GA_Dash

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo,
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

	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (Character == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FName HeroName;
	if (auto* ASC = Cast<UFHS_AbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get()))
	{
		HeroName = ASC->GetNameTag().GetTagName();
	}
	
	const FVector WorldDirection = Character->GetActorRotation().RotateVector(DashDirection);
	DashSpeed.Curve.RowName = HeroName;
	DashDuration.Curve.RowName = HeroName;
	const float Speed = DashSpeed.GetValueAtLevel(GetAbilityLevel());
	const float Duration = DashDuration.GetValueAtLevel(GetAbilityLevel());
	
	UFHS_AT_Dash* Task = UFHS_AT_Dash::Dash(this, WorldDirection, Speed, Duration);
	Task->DashEnded.AddDynamic(this, &UFHS_GA_Dash::OnDashEnded);
	Task->ReadyForActivation();
	
} // ActivateAbility

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_Dash::OnDashEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
	
} // OnDashEnded

// ---------------------------------------------------------------------------------------------------------------------

