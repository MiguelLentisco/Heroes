#pragma once

#include "FHS_GameplayAbility.h"
#include "FHS_GA_Jump.generated.h"

UCLASS()
class HEROES_API UFHS_GA_Jump : public UFHS_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UFHS_GA_Jump();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                                const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	                                FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                           const FGameplayAbilityActivationInfo ActivationInfo,
	                           bool bReplicateCancelAbility) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							   const FGameplayAbilityActivationInfo ActivationInfo) override;

	UPROPERTY(EditDefaultsOnly, Category = Fire, Meta = (Categories = "GameplayCue"))
	FGameplayTag SoundGCTag;
	
protected:
	UFUNCTION()
	void OnCharacterLanded(const FHitResult& Hit);

	void StopJumping() const;
	
}; // UFHS_GA_Jump
