#pragma once

#include "FHS_GameplayAbility.h"
#include "FHS_GA_FireWeapon.generated.h"

class AFHS_BaseProjectile;
class UAnimMontage;

UCLASS()
class HEROES_API UFHS_GA_FireWeapon : public UFHS_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UFHS_GA_FireWeapon();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Fire)
	TSoftClassPtr<AFHS_BaseProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Fire, Meta = (Categories = "GameplayCue"))
	FGameplayTag SoundGCTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fire)
	TSoftObjectPtr<UAnimMontage> FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fire)
	FVector MuzzleOffset = FVector(100.f, .0f, 10.f);
	
}; // UFHS_GA_FireWeapon
