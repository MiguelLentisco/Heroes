#include "FHS_GA_Rewind.h"

#include "AbilitySystemComponent.h"
#include "FHS_RewindComponent.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Effects/FHS_GE_ApplyCooldown.h"
#include "Heroes/Hero/FHS_BaseHero.h"

// ---------------------------------------------------------------------------------------------------------------------

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Cooldown_Ability_Rewind, TEXT("Cooldown.Ability.Rewind"));

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GA_Rewind::UFHS_GA_Rewind()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	bUseScalarCooldown = true;
	CooldownScalar = 15.f;
	CooldownTags.AddTag(TAG_Cooldown_Ability_Rewind.GetTag());
	CooldownGameplayEffectClass = UFHS_GE_ApplyCooldown::StaticClass();
	
	ActivationBlockedTags.AddTag(TAG_Status_Stun.GetTag());
	
} // UFHS_GA_Rewind

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_Rewind::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	auto* Hero = Cast<AFHS_BaseHero>(ActorInfo->AvatarActor.Get());
	if (Hero == nullptr || !Hero->HasAuthority())
	{
		return;
	}
	
	Rewind = Cast<UFHS_RewindComponent>(
		Hero->AddComponentByClass(UFHS_RewindComponent::StaticClass(), false, FTransform::Identity, false));
	Rewind->SetRewindTime(RewindTime.GetValueAtLevel(GetAbilityLevel()));

	// Hack to avoid problems on BeginPlay
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		if (this != nullptr && Rewind != nullptr)
		{
			Rewind->StartRecording();
		}
	});
	
} // OnGiveAbility

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_Rewind::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	if (Rewind != nullptr)
	{
		Rewind->DestroyComponent();
		Rewind.Reset();
	}
	
	Super::OnRemoveAbility(ActorInfo, Spec);
	
} // OnRemoveAbility

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_Rewind::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!HasAuthority(&ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (Rewind == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	Rewind->Rewind();

	const FGameplayTagContainer RewindTags(TAG_Status_Poison);
	ActorInfo->AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(RewindTags);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	
} // ActivateAbility

// ---------------------------------------------------------------------------------------------------------------------
