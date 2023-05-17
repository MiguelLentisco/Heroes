#include "FHS_AbilityMeshData.h"

#include "EnhancedInputComponent.h"
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

	FModifyContextOptions ModifyContextOptions;
	ModifyContextOptions.bForceImmediately = true;
	Subsystem->AddMappingContext(AbilitySetLoaded->InputMappingContext.LoadSynchronous(), AbilitySet->InputPriority,
	                             ModifyContextOptions);
	
} // SetupInput

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_AbilityMeshData::ClearInput(UFHS_AbilitySystemComponent* ASC, APawn* Pawn)
{
	if (ASC == nullptr || Pawn == nullptr)
	{
		return;
	}

	ASC->ClearInputs(AbilitySet.LoadSynchronous(), Cast<UEnhancedInputComponent>(Pawn->InputComponent));

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

