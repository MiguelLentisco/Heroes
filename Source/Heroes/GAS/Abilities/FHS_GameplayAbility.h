#pragma once

#include "NativeGameplayTags.h"
#include "Abilities/GameplayAbility.h"
#include "FHS_GameplayAbility.generated.h"

UCLASS()
class HEROES_API UFHS_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UFHS_GameplayAbility();

#pragma region Costs
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs)
	TMap<FGameplayAttribute, FScalableFloat> AttributeCosts;
	
	virtual UGameplayEffect* GetCostGameplayEffect() const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                       const FGameplayAbilityActivationInfo ActivationInfo) const override;

#pragma endregion // Costs

#pragma region Cooldown

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cooldown)
	bool bUseScalarCooldown = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cooldown,
		Meta = (EditCondition = "bUseScalarCooldown", EditConditionHides))
	FScalableFloat CooldownScalar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cooldown,
		Meta = (EditCondition = "!bUseScalarCooldown", EditConditionHides))
	FGameplayAttribute CooldownAttribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cooldown)
	FGameplayTagContainer CooldownTags;

	UPROPERTY(Transient)
	FGameplayTagContainer TempCooldownTags;
	
	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                           const FGameplayAbilityActivationInfo ActivationInfo) const override;
	

#pragma endregion // Cooldown
	
}; // UFHS_GameplayAbility
