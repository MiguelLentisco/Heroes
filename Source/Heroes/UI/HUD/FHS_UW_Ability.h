#pragma once

#include "FHS_GASListener.h"
#include "Blueprint/UserWidget.h"
#include "Heroes/GAS/FHS_AbilityCommand.h"
#include "FHS_UW_Ability.generated.h"

class UGameplayAbility;

UCLASS(Abstract)
class HEROES_API UFHS_UW_Ability : public UUserWidget, public IFHS_GASListener
{
	GENERATED_BODY()
	
public:
	virtual void SetupWithGAS_Implementation(UAbilitySystemComponent* ASC) override;
	virtual void CleanFromGAS_Implementation(UAbilitySystemComponent* ASC) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS)
	EFHS_AbilityCommand AbilityCommand = EFHS_AbilityCommand::None;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UGameplayAbility> LinkedGA;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = GAS)
	void OnCooldownTagChanged(const FGameplayTag PoisonTag, int32 TagCount);
	
}; // UFHS_UW_Ability
