#include "FHS_PlayerController.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Heroes/Core/FHS_MainGameMode.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/Hero/FHS_BaseHero.h"

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	InPawn->SetOwner(this);
	Hero = GetPawn<AFHS_BaseHero>();
	
	UAbilitySystemComponent* ASC = Hero->GetAbilitySystemComponent();
	ASC->RegisterGameplayTagEvent(TAG_Status_Dead.GetTag()).AddUObject(this, &AFHS_PlayerController::OnPlayerDeadChanged);
	
} // OnPossess

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_PlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	if (HasAuthority())
	{
		return;
	}

	Hero = GetPawn<AFHS_BaseHero>();
	
} // AcknowledgePossession

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_PlayerController::OnPlayerDeadChanged(const FGameplayTag DeadTag, int32 NumCount)
{
	const bool bDead = NumCount > 0;
	ChangeCameraPOV(!bDead);

	if (!bDead)
	{
		return;
	}

	GetWorld()->GetAuthGameMode<AFHS_MainGameMode>()->OnPlayerDead(this);
	
} // OnPlayerDeadChanged

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_PlayerController::ChangeCameraPOV_Implementation(bool bSet1P)
{
	if (Hero == nullptr)
	{
		return;
	}

	Hero->GetCamera3P()->SetActive(!bSet1P);
	Hero->GetFirstPersonCameraComponent()->SetActive(bSet1P);
	Hero->GetMesh1P()->SetVisibility(bSet1P, false);
	Hero->GetMesh()->SetOwnerNoSee(bSet1P);
	
} // ChangeCameraPOV

// ---------------------------------------------------------------------------------------------------------------------


