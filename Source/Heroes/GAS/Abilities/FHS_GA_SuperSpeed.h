#pragma once

#include "FHS_GameplayAbility.h"
#include "FHS_GA_SuperSpeed.generated.h"

class UAbilityTask_WaitDelay;

UCLASS()
class HEROES_API UFHS_GA_SuperSpeed : public UFHS_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UFHS_GA_SuperSpeed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperSpeed)
	FScalableFloat Time = 8.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SuperSpeed)
	FScalableFloat Speed = 600.f;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = Reload, Meta = (Categories = "GameplayCue"))
	FGameplayTag GCTag;
	
}; // UFHS_GA_SuperSpeed
