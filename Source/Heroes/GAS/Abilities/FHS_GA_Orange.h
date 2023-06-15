#pragma once

#include "FHS_GameplayAbility.h"
#include "FHS_GA_Orange.generated.h"

UCLASS()
class HEROES_API UFHS_GA_Orange : public UFHS_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UFHS_GA_Orange();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Orange)
	FScalableFloat Heal = 60.f;

	UPROPERTY(EditDefaultsOnly, Category = Orange, Meta = (Categories = "GameplayCue"))
	FGameplayTag SoundGCTag;
	
}; // UFHS_GA_Orange
