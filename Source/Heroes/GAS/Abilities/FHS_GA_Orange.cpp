#include "FHS_GA_Orange.h"

#include "AbilitySystemComponent.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Effects/FHS_GE_ApplyCooldown.h"
#include "Heroes/GAS/Effects/FHS_GE_Orange.h"

// ---------------------------------------------------------------------------------------------------------------------

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Name_Ability_Orange, TEXT("Name.Ability.Orange"));
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Cooldown_Ability_Orange, TEXT("Cooldown.Ability.Orange"));

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GA_Orange::UFHS_GA_Orange()
{
	AbilityTags.AddTag(TAG_Name_Ability_Orange.GetTag());
	CancelAbilitiesWithTag.AddTag(TAG_Status_AbilityCancelable.GetTag());
	
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;

	CooldownGameplayEffectClass = UFHS_GE_ApplyCooldown::StaticClass();
	bUseScalarCooldown = true;
	CooldownScalar = 15.f;
	CooldownTags.AddTag(TAG_Cooldown_Ability_Orange.GetTag());
	
} // UFHS_GA_Orange

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_Orange::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(UFHS_GE_Orange::StaticClass(), 1,
																	   ASC->MakeEffectContext());
	const int32 AbilityLevel = GetAbilityLevel(Handle, ActorInfo);
	SpecHandle.Data->SetSetByCallerMagnitude(TAG_Data_Heal, Heal.GetValueAtLevel(AbilityLevel));
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	
	ASC->ExecuteGameplayCue(SoundGCTag, ASC->MakeEffectContext());
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	
} // ActivateAbility

// ---------------------------------------------------------------------------------------------------------------------
