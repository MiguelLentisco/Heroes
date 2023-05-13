#pragma once

#include "FHS_AbilitySet.generated.h"

class UInputAction;
class UInputMappingContext;
class UAbilitySystemComponent;
class UGameplayAbility;

UENUM(BlueprintType)
enum class EFHS_AbilityCommand : uint8
{
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

USTRUCT(BlueprintType)
struct HEROES_API FAbilityBindData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowAbstract = false))
	TSoftClassPtr<UGameplayAbility> AbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowAbstract = false))
	TSoftObjectPtr<UInputAction> AbilityInput;
	
}; // FAbilityBindData

UCLASS(Blueprintable)
class HEROES_API UFHS_AbilitySet : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EFHS_AbilityCommand, FAbilityBindData> Abilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* InputMappingContext;
	
}; // FHS_UAbilitySet
