#pragma once

#include "FHS_GameplayAbility.h"
#include "FHS_GA_Reload.generated.h"

class UAbilityTask_WaitDelay;

UCLASS()
class HEROES_API UFHS_GA_Reload : public UFHS_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UFHS_GA_Reload();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Reload)
	FScalableFloat ReloadTime = 2.f;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                                const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	                                FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, Category = Fire, Meta = (Categories = "GameplayCue"))
	FGameplayTag CGTag;

protected:
	UFUNCTION()
	void OnReloadFinished();
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitDelay> ReloadTask;
	
}; // UFHS_GA_Reload
