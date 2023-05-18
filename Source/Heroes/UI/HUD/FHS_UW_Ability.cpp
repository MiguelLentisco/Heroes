#include "FHS_UW_Ability.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Heroes/GAS/FHS_AbilitySystemComponent.h"

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_Ability::SetupWithGAS_Implementation(UAbilitySystemComponent* ASC)
{
	SetVisibility(ESlateVisibility::Collapsed);
	
	if (ASC == nullptr || AbilityCommand == EFHS_AbilityCommand::None)
	{
		return;
	}
	
	const FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromInputID(static_cast<int32>(AbilityCommand));
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

	// Setup CD
	if (const FGameplayTagContainer* CooldownTags = LinkedGA->GetCooldownTags())
	{
		for (auto CooldownTag = CooldownTags->CreateConstIterator(); CooldownTag; ++CooldownTag)
		{
			ASC->RegisterGameplayTagEvent(*CooldownTag, EGameplayTagEventType::AnyCountChange).AddUObject(
				this, &UFHS_UW_Ability::OnCooldownTagChanged);
		}
	}

	// Set name
	FString LeftAux, AbilityShortName;
	LinkedGA->AbilityTags.First().ToString().Split(TEXT("."), &LeftAux, &AbilityShortName, ESearchCase::CaseSensitive,
	                                               ESearchDir::FromEnd);
	SetAbilityName(FText::FromString(AbilityShortName));

	// Find key associated to this ability
	if (auto* FHS_ASC = Cast<UFHS_AbilitySystemComponent>(ASC))
	{
		if (FEnhancedInputActionEventBinding* const* EnhancedInput = FHS_ASC->GetAbilitiesBind().Find(AbilityCommand))
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
	
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
} // SetupWithGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_Ability::CleanFromGAS_Implementation(UAbilitySystemComponent* ASC)
{
	SetVisibility(ESlateVisibility::Collapsed);
	
	if (ASC == nullptr || AbilityCommand == EFHS_AbilityCommand::None || LinkedGA == nullptr)
	{
		return;
	}
	
	if (const FGameplayTagContainer* CooldownTags = LinkedGA->GetCooldownTags())
	{
		for (auto CooldownTag = CooldownTags->CreateConstIterator(); CooldownTag; ++CooldownTag)
		{
			ASC->RegisterGameplayTagEvent(*CooldownTag, EGameplayTagEventType::AnyCountChange).RemoveAll(this);
		}
	}

	LinkedGA.Reset();
	
} // CleanFromGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

