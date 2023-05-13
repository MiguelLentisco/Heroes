#pragma once

#include "FHS_AbilityCommand.generated.h"

UENUM(BlueprintType)
enum class EFHS_AbilityCommand : uint8
{
	None,
	
	// Generic abilities
	Ability_1,
	Ability_2,
	Ability_3,
	Ability_4,
	
	// Specific abilities
	Ability_Jump,
	Ability_Movement,
	Ability_FirePrimary,
	Ability_FireSecondary,
	Ability_SwapWeapon,
	Ability_Ultimate,

	Ability_Confirm,
	Ability_Cancel,
	
}; // EFHS_AbilityCommand