#include "FHS_UW_UltimateAbility.h"

#include "EnhancedInputSubsystems.h"
#include "GameplayEffectExtension.h"
#include "Heroes/GAS/FHS_AbilityCommand.h"
#include "Heroes/GAS/FHS_AbilitySystemComponent.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_UltimateAbility::SetupWithGAS_Implementation(UAbilitySystemComponent* ASC)
{
	IFHS_GASListener::SetupWithGAS_Implementation(ASC);

	if (ASC == nullptr)
	{
		return;
	}

	SetVisibility(ESlateVisibility::Collapsed);

	constexpr EFHS_AbilityCommand Command = EFHS_AbilityCommand::Ability_Ultimate;
	const FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromInputID(static_cast<int32>(Command));
	
	if (AbilitySpec == nullptr)
	{
		return;
	}

	TArray<UGameplayAbility*> GAs = AbilitySpec->GetAbilityInstances();
	if (GAs.IsEmpty())
	{
		return;
	}

	LinkedGA = GAs[0];

	// Set Name
	FString LeftAux, AbilityShortName;
	LinkedGA->AbilityTags.First().ToString().Split(TEXT("."), &LeftAux, &AbilityShortName, ESearchCase::CaseSensitive,
												   ESearchDir::FromEnd);
	SetAbilityName(FText::FromString(AbilityShortName));

	// Find key associated to this ability
	if (auto* FHS_ASC = Cast<UFHS_AbilitySystemComponent>(ASC))
	{
		if (FEnhancedInputActionEventBinding* const* EnhancedInput = FHS_ASC->GetAbilitiesBind().Find(Command))
		{
			const UInputAction* InputAction = (*EnhancedInput)->GetAction();
			if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetOwningLocalPlayer()))
			{
				const TArray<FKey> Keys = Subsystem->QueryKeysMappedToAction(InputAction);
				if (!Keys.IsEmpty())
				{
					SetKeyBindName(Keys[0].GetDisplayName());
				}
			}
		}
	}

	// Callback for energy at max
	const FGameplayAttribute PowerAttribute = UFHS_Attributes_CharacterCore::GetCurrentUltimatePowerAttribute();
	ASC->GetGameplayAttributeValueChangeDelegate(PowerAttribute).AddUObject(
		this, &UFHS_UW_UltimateAbility::OnCurrentUltimatePowerChanged, ASC);
	
} // SetupWithGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_UltimateAbility::CleanFromGAS_Implementation(UAbilitySystemComponent* ASC)
{
	IFHS_GASListener::CleanFromGAS_Implementation(ASC);

	if (ASC == nullptr)
	{
		return;
	}

	ASC->GetGameplayAttributeValueChangeDelegate(UFHS_Attributes_CharacterCore::GetCurrentUltimatePowerAttribute()).
	     RemoveAll(this);
	
} // CleanFromGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_UltimateAbility::OnCurrentUltimatePowerChanged(const FOnAttributeChangeData& Data,
	UAbilitySystemComponent* ASC)
{
	if (ASC == nullptr)
	{
		return;
	}
			
	const FGameplayAttribute MaxPowerAttribute = UFHS_Attributes_CharacterCore::GetMaxUltimatePowerAttribute();
	const float CurrentMaxPower = ASC->GetNumericAttribute(MaxPowerAttribute);
	const bool bHasMaxPower = FMath::IsNearlyEqual(Data.NewValue, CurrentMaxPower, .01f);

	if (bHasMaxPower == bUltimateReady)
	{
		return;
	}

	bUltimateReady = bHasMaxPower;
	OnUltimatePowerReady(bUltimateReady);
	
} // OnCurrentUltimatePowerChanged

// ---------------------------------------------------------------------------------------------------------------------

