#pragma once

#include "AttributeSet.h"
#include "FHS_GASListener.h"
#include "Blueprint/UserWidget.h"
#include "FHS_UW_Attributes.generated.h"

UCLASS(Abstract)
class HEROES_API UFHS_UW_Attributes : public UUserWidget, public IFHS_GASListener
{
	GENERATED_BODY()
	
public:
	virtual void SetupWithGAS_Implementation(UAbilitySystemComponent* ASC,
	                                         const TArray<UAbilitySystemComponent*>& WeaponASCs) override;
	virtual void CleanFromGAS_Implementation(UAbilitySystemComponent* ASC,
	                                         const TArray<UAbilitySystemComponent*>& WeaponASCs) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS)
	float MaxHealth = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS)
	float MaxUltimatePower = 0.f;
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = GAS)
	void OnHealthChanged(FGameplayAttribute Attribute, float NewValue, float OldValue);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = GAS)
	void OnUltimatePowerChanged(FGameplayAttribute Attribute, float NewValue, float OldValue);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = GAS)
	void OnPoisonEffectChanged(const FGameplayTag PoisonTag, int32 TagCount);
	
	
}; // UFHS_UW_Attributes
