#pragma once

#include "Heroes/GAS/Abilities/FHS_GameplayAbility.h"
#include "FHS_GA_UltBomb.generated.h"

UCLASS()
class UFHS_GA_UltBomb : public UFHS_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UFHS_GA_UltBomb();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bomb)
	FVector Offset = FVector(10.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bomb)
	float ThrowInclination = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bomb)
	float Gravity = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bomb)
	FScalableFloat Speed = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bomb)
	FScalableFloat ExplosionRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bomb)
	FScalableFloat MaxDamage = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bomb)
	FScalableFloat ImplodeTime = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bomb)
	TSoftObjectPtr<UCurveFloat> DamageByDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bomb)
	TSoftClassPtr<AActor> BombClass;

	UPROPERTY(EditDefaultsOnly, Category = Bomb, Meta = (Categories = "GameplayCue"))
	FGameplayTag GCTag;
	
}; // UFHS_GA_UltBomb
