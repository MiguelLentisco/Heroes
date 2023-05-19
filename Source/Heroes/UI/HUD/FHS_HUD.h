#pragma once

#include "GameFramework/HUD.h"
#include "GameplayTags.h"
#include "FHS_HUD.generated.h"

class UInputAction;
class UFHS_HeroData;
class UFHS_UW_HeroSelector;
class AFHS_BaseHero;
class AFHS_BaseWeapon;
class UFHS_UW_HUD;
class UInputMappingContext;

UCLASS(Abstract)
class HEROES_API AFHS_HUD : public AHUD
{
	GENERATED_BODY()

public:
	AFHS_HUD();

	void CreateHUD();
	bool IsHUDCreated() const;

	virtual void BeginPlay() override;

	void OpenMenu();
	void OnSafeZone(FGameplayTag ImmunityTag, int32 NumCounts);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UFHS_UW_HUD> MainHUDClass;
	UPROPERTY()
	TObjectPtr<UFHS_UW_HUD> MainHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UUserWidget> MainMenuClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> MainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInputAction> OpenMenuInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInputAction> OpenHeroSelectorInput;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UInputMappingContext> MovementMappingContext;
	
	UPROPERTY()
	TWeakObjectPtr<AFHS_BaseHero> Hero;
	UPROPERTY()
	TWeakObjectPtr<AFHS_BaseWeapon> HeroCurrentWeapon;

	bool bMenuOpen = false;

	void OnHeroInputChangedInput(bool bSet);
	void OnHeroWeaponInputChangedInput(bool bSet);
	void OnHeroCurrentWeaponChanged(AFHS_BaseWeapon* NewWeapon);

#pragma region HeroSelector

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UFHS_UW_HeroSelector> HeroSelectorClass;
	UPROPERTY()
	TObjectPtr<UFHS_UW_HeroSelector> HeroSelector;

	bool bCanToggleHeroSelector = false;
	bool bHeroSelectorOpened = false;

	void OpenHeroSelector(bool bOpen);
	void OnHeroSelect(UFHS_HeroData* HeroSelected);
	
#pragma endregion 
	
}; // AFHS_HUD
