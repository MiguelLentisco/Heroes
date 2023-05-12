#include "FHS_GameplayHUD.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

// ---------------------------------------------------------------------------------------------------------------------

AFHS_GameplayHUD::AFHS_GameplayHUD()
{
	PrimaryActorTick.bCanEverTick = false;
	
} // AFHS_GameplayHUD

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_GameplayHUD::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerOwner == nullptr)
	{
		return;
	}

	const IAbilitySystemInterface* ASInterface = Cast<IAbilitySystemInterface>(PlayerOwner->GetPawn());
	if (ASInterface == nullptr)
	{
		return;
	}

	UAbilitySystemComponent* ASC = ASInterface->GetAbilitySystemComponent();
	if (ASC == nullptr)
	{
		return;
	}
	
	
} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------



