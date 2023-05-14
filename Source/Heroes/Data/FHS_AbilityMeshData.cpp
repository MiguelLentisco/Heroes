#include "FHS_AbilityMeshData.h"

#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "Heroes/GAS/FHS_AbilitySet.h"
#include "Heroes/GAS/FHS_AbilitySystemComponent.h"

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_AbilityMeshData::SetupInput(UFHS_AbilitySystemComponent* ASC, APawn* Pawn)
{
	if (Pawn == nullptr || ASC == nullptr || Pawn->InputComponent == nullptr)
	{
		return;
	}

	const UFHS_AbilitySet* AbilitySetLoaded = AbilitySet.LoadSynchronous();
	if (AbilitySetLoaded == nullptr)
	{
		return;
	}

	ASC->BindAbilitiesToInput(AbilitySetLoaded, Pawn->InputComponent);
	
	const auto* PC = Pawn->GetController<APlayerController>();
	if (PC == nullptr)
	{
		return;
	}
	
	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (Subsystem == nullptr)
	{
		return;
	}
	
	Subsystem->AddMappingContext(AbilitySetLoaded->InputMappingContext.LoadSynchronous(), AbilitySet->InputPriority);
	
} // SetupInput

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_AbilityMeshData::ClearInput(APawn* Pawn)
{
	if (Pawn == nullptr)
	{
		return;
	}

	const auto* PC = Pawn->GetController<APlayerController>();
	if (PC == nullptr)
	{
		return;
	}
	
	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (Subsystem == nullptr)
	{
		return;
	}

	const UFHS_AbilitySet* AbilitySetLoaded = AbilitySet.LoadSynchronous();
	if (AbilitySetLoaded == nullptr)
	{
		return;
	}
	Subsystem->RemoveMappingContext(AbilitySetLoaded->InputMappingContext.LoadSynchronous());
	
} // ClearInput

// ---------------------------------------------------------------------------------------------------------------------

