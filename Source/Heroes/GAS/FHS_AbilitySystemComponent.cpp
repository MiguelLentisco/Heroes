﻿#include "FHS_AbilitySystemComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FHS_AbilitySet.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_AbilitySystemComponent::UFHS_AbilitySystemComponent()
{
	ReplicationMode = EGameplayEffectReplicationMode::Mixed;

} // UFHS_AbilitySystemComponent

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_AbilitySystemComponent::SetupAbilities(UFHS_AbilitySet* AbilitySet, APlayerController* PC)
{
	if (AbilitySet == nullptr)
	{
		return;
	}

	GiveAbilities(AbilitySet);
	BindAbilitiesToInput(AbilitySet, Cast<APawn>(GetOwner())->InputComponent);
	AddInputMappingContext(AbilitySet->InputMappingContext, PC);
	
} // SetupAbilities

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_AbilitySystemComponent::GiveAbilities(UFHS_AbilitySet* AbilitySet)
{
	for (const TPair<EFHS_AbilityCommand, FAbilityBindData>& Ability : AbilitySet->Abilities)
	{
		UClass* AbilityClass = Ability.Value.AbilityClass.LoadSynchronous();
		if (AbilityClass == nullptr)
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, static_cast<int32>(Ability.Key));
		GiveAbility(AbilitySpec);
	}
	
} // GiveAbilities

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_AbilitySystemComponent::BindAbilitiesToInput(UFHS_AbilitySet* AbilitySet, UInputComponent* Input)
{
	if (Input == nullptr)
	{
		return;
	}

	auto* EnhancedInput = Cast<UEnhancedInputComponent>(Input);
	if (EnhancedInput != nullptr)
	{
		BindAbilityActivationToEnhancedInputComponent(AbilitySet, EnhancedInput);
	}
	else
	{
		const FTopLevelAssetPath EnumName(TEXT("/Script/Heroes"), TEXT("EFHS_AbilityCommand"));
		const FGameplayAbilityInputBinds BindInfo(TEXT("Ability_Confirm"), TEXT("Ability_Cancel"), EnumName);
		BindAbilityActivationToInputComponent(Input, BindInfo);
	}
	
} // BindAbilitiesToInput

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_AbilitySystemComponent::BindAbilityActivationToEnhancedInputComponent(
	UFHS_AbilitySet* AbilitySet, UEnhancedInputComponent* EnhancedInput)
{
	const TMap<EFHS_AbilityCommand, FAbilityBindData>& Abilities = AbilitySet->Abilities;
	
	// Same as the original func, reset ability bindings
	bIsNetDirty = true;
	GetBlockedAbilityBindings_Mutable().SetNumZeroed(Abilities.Num());
	
	for (const TPair<EFHS_AbilityCommand, FAbilityBindData>& Ability : AbilitySet->Abilities)
	{
		const int32 AbilityCommandIdx = static_cast<int32>(Ability.Key);
		const TSoftObjectPtr<UInputAction> InputActionPtr = Ability.Value.AbilityInput;
		
		const TObjectPtr<UInputAction> InputAction = InputActionPtr.LoadSynchronous();
		if (InputAction == nullptr)
		{
			continue;
		}
		
		EnhancedInput->BindAction(InputAction, ETriggerEvent::Triggered, this,
		                           &UAbilitySystemComponent::AbilityLocalInputPressed, AbilityCommandIdx);
		EnhancedInput->BindAction(InputAction, ETriggerEvent::Completed, this,
		                           &UAbilitySystemComponent::AbilityLocalInputReleased, AbilityCommandIdx);
	}
	
	if (const FAbilityBindData* ConfirmBind = Abilities.Find(EFHS_AbilityCommand::Ability_Confirm))
	{
		if (const TObjectPtr<UInputAction> ConfirmIA = ConfirmBind->AbilityInput.LoadSynchronous())
		{
			EnhancedInput->BindAction(ConfirmIA, ETriggerEvent::Triggered, this, &UAbilitySystemComponent::LocalInputConfirm);
		}
	}
	
	if (const FAbilityBindData* CancelBind = Abilities.Find(EFHS_AbilityCommand::Ability_Cancel))
	{
		if (const TObjectPtr<UInputAction> CancelIA = CancelBind->AbilityInput.LoadSynchronous())
		{
			EnhancedInput->BindAction(CancelIA, ETriggerEvent::Triggered, this, &UAbilitySystemComponent::LocalInputCancel);
		}
	}
	
} // BindAbilityActivationToEnhancedInputComponent

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_AbilitySystemComponent::AddInputMappingContext(UInputMappingContext* IMC, APlayerController* PC) const
{
	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (Subsystem == nullptr)
	{
		return;
	}
	
	Subsystem->AddMappingContext(IMC, 1);
		
} // AddInputMappingContext

// ---------------------------------------------------------------------------------------------------------------------
