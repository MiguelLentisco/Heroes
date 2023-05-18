#pragma once

#include "FHS_GASListener.h"
#include "Blueprint/UserWidget.h"
#include "FHS_UW_UltimateAbility.generated.h"

UCLASS()
class HEROES_API UFHS_UW_UltimateAbility : public UUserWidget, public IFHS_GASListener
{
	GENERATED_BODY()
	
public:
	virtual void SetupWithGAS_Implementation(UAbilitySystemComponent* ASC) override;
	virtual void CleanFromGAS_Implementation(UAbilitySystemComponent* ASC) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UGameplayAbility> LinkedGA;

	void OnCurrentUltimatePowerChanged(const FOnAttributeChangeData& Data, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintImplementableEvent)
	void SetAbilityName(const FText& AbilityName);
	UFUNCTION(BlueprintImplementableEvent)
	void SetKeyBindName(const FText& KeyBindName);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = GAS)
	void OnUltimatePowerReady(bool bReady);

	bool bUltimateReady = false;
	
}; // UFHS_UW_UltimateAbility
