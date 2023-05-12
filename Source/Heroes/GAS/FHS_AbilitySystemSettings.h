#pragma once

#include "FHS_AbilitySystemSettings.generated.h"

class UInputAction;
enum class EFHS_AbilityCommand : uint8;

UCLASS(Config = Game, DefaultConfig, DisplayName = "Ability System Settings")
class HEROES_API UFHS_AbilitySystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UFHS_AbilitySystemSettings();
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Input")
	TMap<EFHS_AbilityCommand, TSoftObjectPtr<UInputAction>> CommandsToActions;
	
}; // UFHS_AbilitySystemSettings
