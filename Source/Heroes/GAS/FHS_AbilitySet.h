#pragma once

#include "FHS_AbilityCommand.h"
#include "FHS_AbilitySet.generated.h"

class UInputAction;
class UInputMappingContext;
class UAbilitySystemComponent;
class UGameplayAbility;

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
