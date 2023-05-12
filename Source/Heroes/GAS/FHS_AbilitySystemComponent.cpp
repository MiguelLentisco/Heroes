#include "FHS_AbilitySystemComponent.h"

#include "EnhancedInputComponent.h"
#include "FHS_AbilitySet.h"
#include "FHS_AbilitySystemSettings.h"

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_AbilitySystemComponent::GiveAbilities(UFHS_AbilitySet* Abilities)
{
	if (Abilities == nullptr)
	{
		return;
	}

	for (const FAbilityBindData& Ability : Abilities->Abilities)
	{
		UClass* AbilityClass = Ability.AbilityClass.LoadSynchronous();
		if (AbilityClass == nullptr)
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, static_cast<int32>(Ability.AbilityInput));
		GiveAbility(AbilitySpec);
	}
	
} // GiveAbilities

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_AbilitySystemComponent::BindAbilityActivationToEnhancedInputComponent(UEnhancedInputComponent* InputComponent)
{
	const TMap<EFHS_AbilityCommand, TSoftObjectPtr<UInputAction>>& InputMap = GetDefault<UFHS_AbilitySystemSettings>()->CommandsToActions;
	
	// Same as the original func
	bIsNetDirty = true;
	GetBlockedAbilityBindings_Mutable().SetNumZeroed(static_cast<int32>(EFHS_AbilityCommand::MAX));
	
	for (EFHS_AbilityCommand AbilityCommand : TEnumRange<EFHS_AbilityCommand>())
	{
		const int32 AbilityCommandIdx = static_cast<int32>(AbilityCommand);
		const TObjectPtr<UInputAction> InputAction = InputMap[AbilityCommand].Get();
		if (InputAction == nullptr)
		{
			continue;
		}
		
		InputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this,
		                           &UAbilitySystemComponent::AbilityLocalInputPressed, AbilityCommandIdx);
		InputComponent->BindAction(InputAction, ETriggerEvent::Completed, this,
		                           &UAbilitySystemComponent::AbilityLocalInputReleased, AbilityCommandIdx);
	}

	if (const TObjectPtr<UInputAction> ConfirmAbility = InputMap[EFHS_AbilityCommand::Ability_Confirm].Get())
	{
		InputComponent->BindAction(ConfirmAbility, ETriggerEvent::Triggered, this,
		                           &UAbilitySystemComponent::LocalInputConfirm);
	}

	if (const TObjectPtr<UInputAction> CancelAbility = InputMap[EFHS_AbilityCommand::Ability_Cancel].Get())
	{
		InputComponent->BindAction(CancelAbility, ETriggerEvent::Triggered, this,
								   &UAbilitySystemComponent::LocalInputCancel);
	}
	
} // BindAbilityActivationToEnhancedInputComponent

// ---------------------------------------------------------------------------------------------------------------------

