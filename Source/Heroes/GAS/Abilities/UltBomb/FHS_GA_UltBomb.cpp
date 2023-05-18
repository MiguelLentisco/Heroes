#include "FHS_GA_UltBomb.h"

#include "FHS_BombComponent.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"

// ---------------------------------------------------------------------------------------------------------------------

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Name_Ability_Ultimate_Bomb, TEXT("Name.Ability.Ultimate.Bomb"));

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GA_UltBomb::UFHS_GA_UltBomb()
{
	AbilityTags.AddTag(TAG_Name_Ability_Ultimate_Bomb.GetTag());
	
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	CancelAbilitiesWithTag.AddTag(TAG_Name_Ability_Reload.GetTag());
	ActivationBlockedTags.AddTag(TAG_Status_Stunned.GetTag());
	ActivationBlockedTags.AddTag(TAG_Status_Dead.GetTag());

	FGameplayModifierInfo Cost;
	Cost.ModifierOp = EGameplayModOp::Additive;
	Cost.Attribute = UFHS_Attributes_CharacterCore::GetCurrentUltimatePowerAttribute();

	FAttributeBasedFloat MaxPowerCost;
	MaxPowerCost.BackingAttribute = FGameplayEffectAttributeCaptureDefinition(
		UFHS_Attributes_CharacterCore::GetMaxUltimatePowerAttribute(), EGameplayEffectAttributeCaptureSource::Source,
		false);
	MaxPowerCost.Coefficient = -1.f;

	Cost.ModifierMagnitude = MaxPowerCost;
	AttributeCosts = { Cost };
	
} // UFHS_GA_UltBomb

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_UltBomb::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	if (!HasAuthority(&ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || BombClass.IsNull())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Cast<APawn>(ActorInfo->AvatarActor.Get());
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	const FRotator Rotation = FRotator(ThrowInclination, SpawnParameters.Instigator->GetControlRotation().Yaw, 0.f);
	const FVector Location = SpawnParameters.Instigator->GetActorLocation() + Rotation.RotateVector(Offset);
	
	AActor* Bomb = GetWorld()->SpawnActor<AActor>(BombClass.LoadSynchronous(), Location, Rotation, SpawnParameters);
	auto* Projectile = Bomb->FindComponentByClass<UFHS_BombComponent>();
	if (Projectile == nullptr)
	{
		Projectile = Cast<UFHS_BombComponent>(
			Bomb->AddComponentByClass(UFHS_BombComponent::StaticClass(), false, FTransform::Identity, false));
	}

	const int32 AbilityLevel = GetAbilityLevel(Handle, ActorInfo);
	Projectile->ImplodeTime = ImplodeTime.GetValueAtLevel(AbilityLevel);
	Projectile->MaxDamage = MaxDamage.GetValueAtLevel(AbilityLevel);
	Projectile->ExplosionRadius = ExplosionRadius.GetValueAtLevel(AbilityLevel);
	const float SpeedVal = Speed.GetValueAtLevel(AbilityLevel);
	Projectile->Velocity = Projectile->Velocity.GetSafeNormal() * SpeedVal;
	Projectile->MaxSpeed = SpeedVal;
	Projectile->ProjectileGravityScale = Gravity;
	Projectile->DamageByDistance = DamageByDistance.LoadSynchronous();
	Projectile->GCTag = GCTag;

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	
} // ActivateAbility

// ---------------------------------------------------------------------------------------------------------------------
