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

void UFHS_AbilitySystemComponent::RemoveAbilities(UFHS_AbilitySet* AbilitySet)
{
	if (AbilitySet == nullptr)
	{
		return;
	}
	
	for (const TPair<EFHS_AbilityCommand, FAbilityBindData>& Ability : AbilitySet->Abilities)
	{
		const UClass* AbilityClass = Ability.Value.AbilityClass.LoadSynchronous();
		if (AbilityClass == nullptr)
		{
			continue;
		}
		
		ClearAllAbilitiesWithInputID(static_cast<int32>(Ability.Key));
	}
	
} // RemoveAbilities

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

void UFHS_AbilitySystemComponent::ClearInputs(const UFHS_AbilitySet* AbilitySet, UEnhancedInputComponent* EnhancedInput)
{
	if (AbilitySet == nullptr || EnhancedInput == nullptr)
	{
		return;
	}

	for (const TPair<EFHS_AbilityCommand, FAbilityBindData>& Ability : AbilitySet->Abilities)
	{
		EnhancedInput->RemoveBinding(*AbilitiesBind[Ability.Key]);
		AbilitiesBind.Remove(Ability.Key);
	}
	
} // ClearInputs

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_AbilitySystemComponent::BindAbilityActivationToEnhancedInputComponent(
	const UFHS_AbilitySet* AbilitySet, UEnhancedInputComponent* EnhancedInput)
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
		
		AbilitiesBind.Add(Ability.Key, &EnhancedInput->BindAction(InputAction, ETriggerEvent::Triggered, this,
								   &UAbilitySystemComponent::AbilityLocalInputPressed, AbilityCommandIdx));
		AbilitiesBind.Add(Ability.Key, &EnhancedInput->BindAction(InputAction, ETriggerEvent::Completed, this,
								   &UAbilitySystemComponent::AbilityLocalInputReleased, AbilityCommandIdx));
	}
	
	if (const FAbilityBindData* ConfirmBind = Abilities.Find(EFHS_AbilityCommand::Ability_Confirm))
	{
		if (const TObjectPtr<UInputAction> ConfirmIA = ConfirmBind->AbilityInput.LoadSynchronous())
		{
			AbilitiesBind.Add(EFHS_AbilityCommand::Ability_Confirm,
			                  &EnhancedInput->BindAction(ConfirmIA, ETriggerEvent::Triggered, this,
			                                            &UAbilitySystemComponent::LocalInputConfirm));
		}
	}
	
	if (const FAbilityBindData* CancelBind = Abilities.Find(EFHS_AbilityCommand::Ability_Cancel))
	{
		if (const TObjectPtr<UInputAction> CancelIA = CancelBind->AbilityInput.LoadSynchronous())
		{
			AbilitiesBind.Add(EFHS_AbilityCommand::Ability_Cancel,
			                  &EnhancedInput->BindAction(CancelIA, ETriggerEvent::Triggered, this,
			                                             &UAbilitySystemComponent::LocalInputCancel));
		}
	}
	
} // BindAbilityActivationToEnhancedInputComponent

// ---------------------------------------------------------------------------------------------------------------------