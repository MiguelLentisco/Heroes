#include "FHS_AT_Dash.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_AT_Dash* UFHS_AT_Dash::Dash(UGameplayAbility* OwningAbility, const FVector& Direction, float Speed,
	float Duration)
{
	UFHS_AT_Dash* NewAT = NewAbilityTask<UFHS_AT_Dash>(OwningAbility);
	
	NewAT->Speed = Speed;
	NewAT->Direction = Direction.GetSafeNormal();
	NewAT->Duration = Duration;
	
	return NewAT;
	
} // Dash

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_AT_Dash::Activate()
{
	Super::Activate();

	const ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
	if (Character == nullptr)
	{
		EndTask();
		return;
	}

	CMC = Character->GetCharacterMovement();
	if (CMC == nullptr)
	{
		EndTask();
		return;
	}

	if (AController* PawnController = CMC->GetPawnOwner()->GetController())
	{
		PawnController->SetIgnoreMoveInput(true);
	}
	
	CMC->Velocity = Speed * Direction;
	CMC->SetMovementMode(MOVE_Flying);

	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UFHS_AT_Dash::OnTimeEnded, Duration);
	
} // Activate

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_AT_Dash::OnDestroy(bool bInOwnerFinished)
{
	RestoreCMC();
	GetWorld()->GetTimerManager().ClearTimer(Timer);
	
	Super::OnDestroy(bInOwnerFinished);
	
} // OnDestroy

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_AT_Dash::RestoreCMC() const
{
	if (CMC == nullptr)
	{
		return;
	}

	if (AController* PawnController = CMC->GetPawnOwner()->GetController())
	{
		PawnController->SetIgnoreMoveInput(false);
	}
	CMC->SetMovementMode(MOVE_Walking);
	CMC->Velocity *= 0.25f;
	
} // RestoreCMC

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_AT_Dash::OnTimeEnded()
{
	EndTask();
	DashEnded.Broadcast();
	
} // OnTimeEnded

// ---------------------------------------------------------------------------------------------------------------------

