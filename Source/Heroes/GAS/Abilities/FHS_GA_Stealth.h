#pragma once

#include "FHS_GameplayAbility.h"
#include "FHS_GA_Stealth.generated.h"

class UAbilityTask_WaitDelay;

UCLASS()
class HEROES_API UFHS_GA_Stealth : public UFHS_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UFHS_GA_Stealth();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stealth)
	FScalableFloat StealthTime = 2.f;

	UPROPERTY(EditDefaultsOnly, Category = Stealth, Meta = (Categories = "GameplayCue"))
	FGameplayTag GCTag;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
							bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnStealthEnded();
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitDelay> StealthWaitTask;
	
}; // UFHS_GA_Reload
