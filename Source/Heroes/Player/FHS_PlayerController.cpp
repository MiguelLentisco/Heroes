#include "FHS_PlayerController.h"

#include "Heroes/Hero/FHS_BaseHero.h"

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	InPawn->SetOwner(this);
	
} // OnPossess

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && IsRunningDedicatedServer())
	{
		return;
	}
	

	
} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------


