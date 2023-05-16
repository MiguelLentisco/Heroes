#pragma once

#include "GameFramework/HUD.h"
#include "FHS_HUD.generated.h"

class AFHS_BaseHero;
class AFHS_BaseWeapon;
class UFHS_UW_HUD;

UCLASS(Abstract)
class HEROES_API AFHS_HUD : public AHUD
{
	GENERATED_BODY()

public:
	AFHS_HUD();

	void CreateHUD();
	bool IsHUDCreated() const;

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UFHS_UW_HUD> MainHUDClass;

	UPROPERTY()
	TObjectPtr<UFHS_UW_HUD> MainHUD;

	UPROPERTY()
	TWeakObjectPtr<AFHS_BaseHero> Hero;
	UPROPERTY()
	TWeakObjectPtr<AFHS_BaseWeapon> HeroCurrentWeapon;

	void OnHeroInputChangedInput(bool bSet);
	void OnHeroWeaponInputChangedInput(bool bSet);
	void OnHeroCurrentWeaponChanged(AFHS_BaseWeapon* NewWeapon);
	
}; // AFHS_HUD
