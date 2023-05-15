#pragma once

#include "GameplayEffect.h"
#include "FHS_GE_ApplyCooldown.generated.h"

/**
 * @brief General GE for applying cooldown on a @UFHS_GameplayAbility
 */
UCLASS()
class HEROES_API UFHS_GE_ApplyCooldown : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UFHS_GE_ApplyCooldown();
	
}; // UFHS_GE_ApplyCooldown
