#pragma once

#include "GameplayModMagnitudeCalculation.h"
#include "FHS_MMC_Cooldown.generated.h"

/**
 * @brief MMC that returns the cooldown defined on @UFHS_GameplayAbility
 */
UCLASS()
class HEROES_API UFHS_MMC_Cooldown : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
}; // UFHS_MMC_Cooldown
