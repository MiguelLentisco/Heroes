#include "FHS_PlayerController.h"

#include "FHS_PlayerState.h"
#include "Heroes/Hero/FHS_BaseHero.h"
#include "Heroes/UI/HUD/FHS_HUD.h"

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	auto* Hero = Cast<AFHS_BaseHero>(InPawn);
	const auto* PS = GetPlayerState<AFHS_PlayerState>();
	
	if (Hero == nullptr || PS == nullptr)
	{
		return;
	}

	Hero->SetOwner(this);
	if (Hero->SetHeroData(PS->ChosenHeroData))
	{
		SetupHUD(InPawn);
	}
	
} // OnPossess

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_PlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	// Do nothing for servers
	if (GetWorld()->GetNetMode() < NM_Client)
	{
		return;
	}

	auto* Hero = Cast<AFHS_BaseHero>(P);
	const auto* PS = GetPlayerState<AFHS_PlayerState>();
	
	if (Hero == nullptr || PS == nullptr)
	{
		return;
	}
	
	if (Hero->SetHeroData(PS->ChosenHeroData))
	{
		SetupHUD(P);
	}
	
} // AcknowledgePossession


// ---------------------------------------------------------------------------------------------------------------------

void AFHS_PlayerController::SetupHUD(APawn* P)
{
	const auto* Hero = Cast<AFHS_BaseHero>(P);
	AFHS_HUD* HUD = GetHUD<AFHS_HUD>();
	if (Hero == nullptr || HUD == nullptr)
	{
		return;
	}

	HUD->CreateHUD();
	IFHS_GASListener::Execute_SetupWithGAS(HUD, Hero->GetAbilitySystemComponent());
	
} // SetupHUD

// ---------------------------------------------------------------------------------------------------------------------

