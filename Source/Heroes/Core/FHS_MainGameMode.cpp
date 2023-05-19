#include "FHS_MainGameMode.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Heroes/GAS/FHS_GameplayTags.h"

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_MainGameMode::OnPlayerDead(APlayerController* PC)
{
	if (PC == nullptr)
	{
		return;
	}

	FTimerHandle Timer;
	const FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AFHS_MainGameMode::RespawnPlayerDead, PC);
	GetWorldTimerManager().SetTimer(Timer, Delegate, RespawnTime, false);
	
} // RespawnDeadPlayer

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_MainGameMode::RespawnPlayerDead(APlayerController* PC)
{
	if (PC == nullptr)
	{
		return;
	}

	auto* Hero = PC->GetPawn<IAbilitySystemInterface>();
	if (Hero == nullptr)
	{
		return;
	}
	
	Hero->GetAbilitySystemComponent()->RemoveActiveEffectsWithAppliedTags(FGameplayTagContainer(TAG_Status_Dead.GetTag()));
	const AActor* const StartSpot = FindPlayerStart(PC, FString());
	if (StartSpot == nullptr)
	{
		return;
	}
	
	const FRotator Rotation = StartSpot->GetActorRotation();
	PC->GetPawn()->SetActorLocationAndRotation(StartSpot->GetActorLocation(), Rotation);
	PC->SetControlRotation(Rotation);
	
} // RespawnPlayerDead

// ---------------------------------------------------------------------------------------------------------------------