#include "FHS_GE_ApplyCooldown.h"

#include "MMC/FHS_MMC_Cooldown.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GE_ApplyCooldown::UFHS_GE_ApplyCooldown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FCustomCalculationBasedFloat MMC;
	MMC.CalculationClassMagnitude = UFHS_MMC_Cooldown::StaticClass();
	DurationMagnitude = MMC;
	
} // UFHS_GE_ApplyCooldown

// ---------------------------------------------------------------------------------------------------------------------

