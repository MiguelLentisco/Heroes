#pragma once

#include "Blueprint/UserWidget.h"
#include "FHS_UW_HUD.generated.h"

class UFHS_UW_UltimateAbility;
class UFHS_UW_Attributes;
class UFHS_UW_Abilities;
class UFHS_UW_Weapon;
class UAbilitySystemComponent;

UCLASS(Abstract)
class HEROES_API UFHS_UW_HUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void RegisterCallbacks();
	virtual void BeginDestroy() override;
	
	void OnHeroInputChangedInput(UAbilitySystemComponent* ASC, bool bSet);
	void OnHeroWeaponInputChangedInput(UAbilitySystemComponent* ASC, bool bSet);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnDeadChanged(const FGameplayTag Tag, int32 TagCount);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnStunChanged(const FGameplayTag Tag, int32 TagCount);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetNumKills(int32 NumKills);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetNumDeaths(int32 NumDeaths);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UFHS_UW_Attributes> AttributesHUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UFHS_UW_Abilities> AbilitiesHUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UFHS_UW_Weapon> WeaponHUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UFHS_UW_UltimateAbility> UltimateAbilityHUD;

	FTimerHandle SetCallbacksTimer;

	void TryBindCallbacks();
	
}; // UFHS_UW_HUD
