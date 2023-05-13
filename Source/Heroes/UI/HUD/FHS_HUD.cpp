#include "FHS_HUD.h"

#include "FHS_UW_HUD.h"
#include "Blueprint/UserWidget.h"

// ---------------------------------------------------------------------------------------------------------------------

AFHS_HUD::AFHS_HUD()
{
	PrimaryActorTick.bCanEverTick = false;
	
} // AFHS_HUD

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_HUD::SetupWithGAS_Implementation(UAbilitySystemComponent* ASC)
{
	if (ASC == nullptr || MainHUD == nullptr)
	{
		return;
	}
	
	IFHS_GASListener::Execute_SetupWithGAS(MainHUD, ASC);
	MainHUD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
} // SetupWithGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_HUD::CleanFromGAS_Implementation(UAbilitySystemComponent* ASC)
{
	if (ASC == nullptr || MainHUD == nullptr)
	{
		return;
	}

	IFHS_GASListener::Execute_CleanFromGAS(MainHUD, ASC);
	MainHUD->SetVisibility(ESlateVisibility::Collapsed);
	
} // CleanFromGAS_Implementation

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
	
} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------



