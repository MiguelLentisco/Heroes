#pragma once

#include "FHS_GameplayAbility.h"
#include "FHS_GA_Dash.generated.h"

UCLASS(Blueprintable)
class HEROES_API UFHS_GA_Dash : public UFHS_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UFHS_GA_Dash();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash)
	FVector DashDirection = FVector(1.f, 0.f, 0.f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash)
	FScalableFloat DashSpeed = FScalableFloat(2500.f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash)
	FScalableFloat DashDuration = FScalableFloat(.2f);

	UPROPERTY(EditDefaultsOnly, Category = Fire, Meta = (Categories = "GameplayCue"))
	FGameplayTag CGTag;
	
	UFUNCTION()
	void OnDashEnded();
	
}; // UFHS_GA_Dash
