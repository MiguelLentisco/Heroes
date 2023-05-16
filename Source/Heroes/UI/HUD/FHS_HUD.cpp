#include "FHS_HUD.h"

#include "FHS_UW_HUD.h"
#include "Blueprint/UserWidget.h"
#include "Heroes/Hero/FHS_BaseHero.h"
#include "Heroes/Weapons/FHS_BaseWeapon.h"

// ---------------------------------------------------------------------------------------------------------------------

AFHS_HUD::AFHS_HUD()
{
	PrimaryActorTick.bCanEverTick = false;
	
} // AFHS_HUD

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_HUD::CreateHUD()
{
	if (IsHUDCreated())
	{
		return;
	}
	
	UClass* MainHUDClassLoaded = MainHUDClass.LoadSynchronous();
	if (PlayerOwner == nullptr || MainHUDClassLoaded == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[AFHS_HUD - BeginPlay] Could create MainHUD due to PC or HUDClass null"))
		return;
	}

	MainHUD = CreateWidget<UFHS_UW_HUD, APlayerController*>(PlayerOwner, MainHUDClassLoaded);
	ensureAlwaysMsgf(MainHUD != nullptr, TEXT("MainHUD couldn't be created"));
	MainHUD->AddToPlayerScreen();
	MainHUD->SetVisibility(ESlateVisibility::Collapsed);
	
} // CreateHUD

// ---------------------------------------------------------------------------------------------------------------------

bool AFHS_HUD::IsHUDCreated() const
{
	return MainHUD != nullptr;
	
} // IsHUDCreated

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_HUD::BeginPlay()
{
	Super::BeginPlay();

	CreateHUD();

	Hero = Cast<AFHS_BaseHero>(GetOwningPawn());
	if (Hero == nullptr)
	{
		return;
	}

	Hero->OnHeroInputChanged.AddUObject(this, &AFHS_HUD::OnHeroInputChangedInput);
	Hero->OnMainWeaponChanged.AddUObject(this, &AFHS_HUD::OnHeroCurrentWeaponChanged);
	
	if (Hero->IsInputSet())
	{
		OnHeroInputChangedInput(true);
	}

	OnHeroCurrentWeaponChanged(Hero->GetCurrentWeapon());
	
} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_HUD::OnHeroInputChangedInput(bool bSet)
{
	MainHUD->SetVisibility(bSet ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (Hero != nullptr)
	{
		MainHUD->OnHeroInputChangedInput(Hero->GetAbilitySystemComponent(), bSet);
	}
	
} // OnHeroInputChangedInput

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_HUD::OnHeroWeaponInputChangedInput(bool bSet)
{
	if (HeroCurrentWeapon == nullptr)
	{
		return;
	}

	MainHUD->OnHeroWeaponInputChangedInput(HeroCurrentWeapon->GetAbilitySystemComponent(), bSet);
	
} // OnHeroWeaponInputChangedInput

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_HUD::OnHeroCurrentWeaponChanged(AFHS_BaseWeapon* NewWeapon)
{
	if (HeroCurrentWeapon != nullptr)
	{
		HeroCurrentWeapon->OnWeaponInputChanged.RemoveAll(this);
		OnHeroWeaponInputChangedInput(false);
	}

	HeroCurrentWeapon = NewWeapon;
	if (HeroCurrentWeapon == nullptr)
	{
		return;
	}

	HeroCurrentWeapon->OnWeaponInputChanged.AddUObject(this, &AFHS_HUD::OnHeroWeaponInputChangedInput);
	if (HeroCurrentWeapon->IsInputSet())
	{
		OnHeroWeaponInputChangedInput(true);
	}
	
} // OnHeroCurrentWeaponChanged

// ---------------------------------------------------------------------------------------------------------------------



