#pragma once

#include "Heroes/GAS/Abilities/FHS_GameplayAbility.h"
#include "FHS_GA_Rewind.generated.h"

class UFHS_RewindComponent;

UCLASS()	
class UFHS_GA_Rewind : public UFHS_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UFHS_GA_Rewind();

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rewind)
	FScalableFloat RewindTime = 6.f;

protected:
	UPROPERTY()
	TWeakObjectPtr<UFHS_RewindComponent> Rewind;
	
}; // UFHS_GA_Rewind
