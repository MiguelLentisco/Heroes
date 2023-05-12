#pragma once

#include "Abilities/GameplayAbility.h"
#include "FHS_GameplayAbility.generated.h"

UCLASS()
class HEROES_API UFHS_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UFHS_GameplayAbility();

#pragma region Cooldown

public:
	const FScalableFloat& GetCooldown() const { return Cooldown; }
	
	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                           const FGameplayAbilityActivationInfo ActivationInfo) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category  = Cooldowns)
	FScalableFloat Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category  = Cooldowns)
	FGameplayTagContainer CooldownTags;

	UPROPERTY(Transient)
	FGameplayTagContainer TempCooldownTags;

#pragma endregion // Cooldown
	
}; // UFHS_GameplayAbility
