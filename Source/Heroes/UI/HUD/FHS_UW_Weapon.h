#pragma once

#include "FHS_GASListener.h"
#include "Blueprint/UserWidget.h"
#include "FHS_UW_Weapon.generated.h"

UCLASS()
class UFHS_UW_Weapon : public UUserWidget, public IFHS_GASListener
{
	GENERATED_BODY()
	
public:
	virtual void SetupWithGAS_Implementation(UAbilitySystemComponent* ASC) override;
	virtual void CleanFromGAS_Implementation(UAbilitySystemComponent* ASC) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GAS)
	int32 AmmoClip = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GAS)
	int32 CurrentAmmo = 0;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = GAS)
	void SetupReady();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = GAS)
	void OnCurrentAmmoChanged(FGameplayAttribute Attribute, float NewValue, float OldValue);
	
}; // UFHS_UW_Weapon
