#include "FHS_GA_FireWeapon.h"

#include "AbilitySystemComponent.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_Weapon.h"
#include "Heroes/GAS/Effects/FHS_GE_ApplyCooldown.h"
#include "Heroes/Weapons/FHS_BaseWeapon.h"

// ---------------------------------------------------------------------------------------------------------------------

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Cooldown_Ability_FireWeapon, TEXT("Cooldown.Ability.FireWeapon"));
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GameplayCue_Ability_FireWeapon, TEXT("GameplayCue.Ability.FireWeapon"));

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GA_FireWeapon::UFHS_GA_FireWeapon()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ActivationBlockedTags.AddTag(TAG_Status_Stun.GetTag());
	CooldownGameplayEffectClass = UFHS_GE_ApplyCooldown::StaticClass();
	bUseScalarCooldown = false;
	CooldownAttribute = UFHS_Attributes_Weapon::GetFireRateAttribute();
	CooldownTags.AddTag(TAG_Cooldown_Ability_FireWeapon.GetTag());
	
} // UFHS_GA_FireWeapon

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_FireWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	auto* Weapon = Cast<AFHS_BaseWeapon>(ActorInfo->AvatarActor.Get());
	if (Weapon == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (HasAuthority(&ActivationInfo))
	{
		Weapon->PrimaryFire();

		FGameplayCueParameters CueParameters;
		CueParameters.Instigator = Weapon;
		CueParameters.Location = Weapon->GetActorLocation();
		ActorInfo->AbilitySystemComponent->ExecuteGameplayCue(TAG_GameplayCue_Ability_FireWeapon.GetTag(), CueParameters);
	}
	
	Weapon->PlayFireMontage();
	
} // ActivateAbility

// ---------------------------------------------------------------------------------------------------------------------
