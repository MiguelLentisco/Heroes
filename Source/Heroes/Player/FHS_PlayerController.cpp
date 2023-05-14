#include "FHS_PlayerController.h"

#include "Heroes/Hero/FHS_BaseHero.h"
#include "Heroes/UI/HUD/FHS_HUD.h"

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	InPawn->SetOwner(this);
	
} // OnPossess

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_PlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	if (GetNetMode() < NM_Client && IsRunningDedicatedServer())
	{
		return;
	}

	auto* Hero = Cast<AFHS_BaseHero>(P);
	if (Hero == nullptr)
	{
		return;
	}
	
	Hero->OnHeroReady.BindUObject(this, &AFHS_PlayerController::SetupHUD);
	SetupHUD();
	
} // AcknowledgePossession

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_PlayerController::SetupHUD()
{
	const auto* Hero = Cast<AFHS_BaseHero>(GetPawn());
	AFHS_HUD* HUD = GetHUD<AFHS_HUD>();
	if (Hero == nullptr || HUD == nullptr)
	{
		return;
	}

	HUD->CreateHUD();
	IFHS_GASListener::Execute_SetupWithGAS(HUD, Hero->GetAbilitySystemComponent());
	
} // SetupHUD

// ------------------------------ ---------------------------------------------------------------------------------------

void AFHS_PlayerController::ClearHUD()
{
	const auto* Hero = Cast<AFHS_BaseHero>(GetPawn());
	AFHS_HUD* HUD = GetHUD<AFHS_HUD>();
	if (Hero == nullptr || HUD == nullptr)
	{
		return;
	}
	
	IFHS_GASListener::Execute_CleanFromGAS(HUD, Hero->GetAbilitySystemComponent());
	
} // ClearHUD

// ---------------------------------------------------------------------------------------------------------------------

