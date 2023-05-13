#include "FHS_PlayerController.h"

#include "FHS_PlayerState.h"
#include "Heroes/Hero/FHS_BaseHero.h"
#include "Heroes/UI/HUD/FHS_HUD.h"

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_PlayerController::OnPossess(APawn* P)
{
	Super::OnPossess(P);

	auto* Hero = Cast<AFHS_BaseHero>(P);
	auto* PS = GetPlayerState<AFHS_PlayerState>();
	
	if (Hero == nullptr || PS == nullptr)
	{
		return;
	}

	Hero->SetOwner(this);
	Hero->SetupHero(PS->ChosenHeroData);

	AFHS_HUD* HUD = GetHUD<AFHS_HUD>();
	if (HUD == nullptr)
	{
		return;
	}

	HUD->CreateHUD();
	IFHS_GASListener::Execute_SetupWithGAS(HUD, Hero->GetAbilitySystemComponent());
	
} // AcknowledgePossession

// ---------------------------------------------------------------------------------------------------------------------

