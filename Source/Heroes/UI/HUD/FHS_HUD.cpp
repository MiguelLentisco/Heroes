#include "FHS_HUD.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FHS_UW_HUD.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/Hero/FHS_BaseHero.h"
#include "Heroes/UI/HeroSelector/FHS_UW_HeroSelector.h"
#include "Heroes/Weapons/FHS_BaseWeapon.h"

// ---------------------------------------------------------------------------------------------------------------------

class UEnhancedInputLocalPlayerSubsystem;

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
	UClass* HeroSelectorClassLoaded = HeroSelectorClass.LoadSynchronous();
	UClass* MenuClassLoaded = MainMenuClass.LoadSynchronous();
	if (PlayerOwner == nullptr || MainHUDClassLoaded == nullptr || HeroSelectorClassLoaded == nullptr || MenuClassLoaded
		== nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[AFHS_HUD - BeginPlay] Could create MainHUD due to PC or HUDClass null"))
		return;
	}

	MainHUD = CreateWidget<UFHS_UW_HUD, APlayerController*>(PlayerOwner, MainHUDClassLoaded);
	MainHUD->AddToPlayerScreen(0);
	MainHUD->SetVisibility(ESlateVisibility::Collapsed);
	
	HeroSelector = CreateWidget<UFHS_UW_HeroSelector, APlayerController*>(PlayerOwner, HeroSelectorClassLoaded);
	HeroSelector->AddToPlayerScreen(1);
	HeroSelector->OnHeroSelected.BindUObject(this, &AFHS_HUD::OnHeroSelect);

	MainMenu = CreateWidget<UUserWidget, APlayerController*>(PlayerOwner, MenuClassLoaded);
	MainMenu->AddToPlayerScreen(2);
	MainMenu->SetVisibility(ESlateVisibility::Collapsed);

	OpenHeroSelector(true);
	
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

	Hero->GetAbilitySystemComponent()->RegisterGameplayTagEvent(TAG_Status_Immunity).AddUObject(
		this, &AFHS_HUD::OnSafeZone);
	bCanToggleHeroSelector = Hero->GetAbilitySystemComponent()->HasMatchingGameplayTag(TAG_Status_Immunity);
	Hero->OnHeroInputChanged.AddUObject(this, &AFHS_HUD::OnHeroInputChangedInput);
	Hero->OnMainWeaponChanged.AddUObject(this, &AFHS_HUD::OnHeroCurrentWeaponChanged);
	
	if (Hero->IsInputSet())
	{
		OnHeroInputChangedInput(true);
	}

	OnHeroCurrentWeaponChanged(Hero->GetCurrentWeapon());

	// Input
	EnableInput(GetOwningPlayerController());
	auto* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInput->BindAction(OpenMenuInput, ETriggerEvent::Triggered, this, &AFHS_HUD::OpenMenu);
	EnhancedInput->BindAction(OpenHeroSelectorInput, ETriggerEvent::Triggered, this, &AFHS_HUD::OpenHeroSelector, false);
	auto* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerOwner->GetLocalPlayer());
	InputSubsystem->AddMappingContext(MovementMappingContext, 5);
	
} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_HUD::OpenMenu()
{
	APlayerController* Controller = GetOwningPlayerController();
	
	MainMenu->SetVisibility(ESlateVisibility::Visible);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(Controller, MainMenu);
	Controller->SetShowMouseCursor(true);
	
} // OpenMenu

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_HUD::OnSafeZone(FGameplayTag ImmunityTag, int32 NumCounts)
{
	bCanToggleHeroSelector = NumCounts > 0;
	
} // OnSafeZone

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

	MainHUD->OnHeroWeaponInputChangedInput(Hero->GetAbilitySystemComponent(), bSet);
	
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

#pragma region HeroSelector


void AFHS_HUD::OpenHeroSelector(bool bForce)
{
	if (!bForce && !bCanToggleHeroSelector)
	{
		return;
	}

	bHeroSelectorOpened = !bHeroSelectorOpened;

	APlayerController* Controller = GetOwningPlayerController();
	if (bHeroSelectorOpened)
	{
		HeroSelector->SetVisibility(ESlateVisibility::Visible);
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(Controller, HeroSelector);
		Controller->SetShowMouseCursor(true);
	}
	else
	{
		HeroSelector->SetVisibility(ESlateVisibility::Collapsed);
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller);
		Controller->SetShowMouseCursor(false);
	}
	
} // OpenHeroSelector

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_HUD::OnHeroSelect(UFHS_HeroData* HeroSelected)
{
	if (HeroSelected == nullptr)
	{
		return;
	}

	Hero->SetHeroData(HeroSelected);
	OpenHeroSelector(true);
	MainHUD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
} // OnHeroSelect

#pragma endregion // HeroSelector

// ---------------------------------------------------------------------------------------------------------------------



