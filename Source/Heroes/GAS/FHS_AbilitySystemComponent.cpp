#include "FHS_AbilitySystemComponent.h"

#include "EnhancedInputSubsystems.h"
#include "FHS_AbilitySet.h"
#include "Net/UnrealNetwork.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_AbilitySystemComponent::UFHS_AbilitySystemComponent()
{
	ReplicationMode = EGameplayEffectReplicationMode::Mixed;

} // UFHS_AbilitySystemComponent

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_AbilitySystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(UFHS_AbilitySystemComponent, NameTag, COND_SimulatedOnly );
	
} // GetLifetimeReplicatedProps

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_AbilitySystemComponent::Clear()
{
	NameTag = FGameplayTag();
	ClearAllAbilities();
	RemoveAllSpawnedAttributes();
	ActiveGameplayEffects.RemoveActiveEffects({}, -1);
	
} // Clear

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_AbilitySystemComponent::GiveAbilities(UFHS_AbilitySet* AbilitySet)
{
	if (AbilitySet == nullptr)
	{
		return;
	}
	
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

void UFHS_AbilitySystemComponent::BindAbilitiesToInput(const UFHS_AbilitySet* AbilitySet, UInputComponent* Input)
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

void UFHS_AbilitySystemComponent::ClearInputs(UEnhancedInputComponent* EnhancedInput)
{
	if (EnhancedInput == nullptr)
	{
		return;
	}

	for (const FInputBindingHandle& InputBinding : AbilitiesBind)
	{
		EnhancedInput->RemoveBinding(InputBinding);
	}
	AbilitiesBind.Empty();
	
} // ClearInputs

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_AbilitySystemComponent::BindAbilityActivationToEnhancedInputComponent(
	const UFHS_AbilitySet* AbilitySet, UEnhancedInputComponent* EnhancedInput)
{
	const TMap<EFHS_AbilityCommand, FAbilityBindData>& Abilities = AbilitySet->Abilities;
	
	// Same as the original func, reset ability bindings
	bIsNetDirty = true;
	GetBlockedAbilityBindings_Mutable().SetNumZeroed(Abilities.Num());

	ClearInputs(EnhancedInput);
	
	for (const TPair<EFHS_AbilityCommand, FAbilityBindData>& Ability : AbilitySet->Abilities)
	{
		const int32 AbilityCommandIdx = static_cast<int32>(Ability.Key);
		const TSoftObjectPtr<UInputAction> InputActionPtr = Ability.Value.AbilityInput;
		
		const TObjectPtr<UInputAction> InputAction = InputActionPtr.LoadSynchronous();
		if (InputAction == nullptr)
		{
			continue;
		}
		
		AbilitiesBind.Add(EnhancedInput->BindAction(InputAction, ETriggerEvent::Triggered, this,
								   &UAbilitySystemComponent::AbilityLocalInputPressed, AbilityCommandIdx));
		AbilitiesBind.Add(EnhancedInput->BindAction(InputAction, ETriggerEvent::Completed, this,
								   &UAbilitySystemComponent::AbilityLocalInputReleased, AbilityCommandIdx));
	}
	
	if (const FAbilityBindData* ConfirmBind = Abilities.Find(EFHS_AbilityCommand::Ability_Confirm))
	{
		if (const TObjectPtr<UInputAction> ConfirmIA = ConfirmBind->AbilityInput.LoadSynchronous())
		{
			AbilitiesBind.Add(EnhancedInput->BindAction(ConfirmIA, ETriggerEvent::Triggered, this,
			                                            &UAbilitySystemComponent::LocalInputConfirm));
		}
	}
	
	if (const FAbilityBindData* CancelBind = Abilities.Find(EFHS_AbilityCommand::Ability_Cancel))
	{
		if (const TObjectPtr<UInputAction> CancelIA = CancelBind->AbilityInput.LoadSynchronous())
		{
			AbilitiesBind.Add(EnhancedInput->BindAction(CancelIA, ETriggerEvent::Triggered, this,
			                                            &UAbilitySystemComponent::LocalInputCancel));
		}
	}
	
} // BindAbilityActivationToEnhancedInputComponent

// ---------------------------------------------------------------------------------------------------------------------