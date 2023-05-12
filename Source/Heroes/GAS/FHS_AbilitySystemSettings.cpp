#include "FHS_AbilitySystemSettings.h"

#include "FHS_AbilitySet.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_AbilitySystemSettings::UFHS_AbilitySystemSettings()
{
	for (EFHS_AbilityCommand AbilityCommand : TEnumRange<EFHS_AbilityCommand>())
	{
		CommandsToActions.FindOrAdd(AbilityCommand);
	}
	
} // UFHS_AbilitySystemSettings

// ---------------------------------------------------------------------------------------------------------------------

