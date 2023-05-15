#include "FHS_UW_Ability.h"

#include "AbilitySystemComponent.h"

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_Ability::SetupWithGAS_Implementation(UAbilitySystemComponent* ASC,
                                                  const TArray<UAbilitySystemComponent*>& WeaponASCs)
{
	SetVisibility(ESlateVisibility::Hidden);
	
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
	const FGameplayTagContainer* CooldownTags = LinkedGA->GetCooldownTags();
	if (CooldownTags == nullptr)
	{
		return;
	}

	for (auto CooldownTag = CooldownTags->CreateConstIterator(); CooldownTag; ++CooldownTag)
	{
		ASC->RegisterGameplayTagEvent(*CooldownTag, EGameplayTagEventType::AnyCountChange).AddUObject(
			this, &UFHS_UW_Ability::OnCooldownTagChanged);
	}

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
} // SetupWithGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_Ability::CleanFromGAS_Implementation(UAbilitySystemComponent* ASC,
                                                  const TArray<UAbilitySystemComponent*>& WeaponASCs)
{
	if (ASC == nullptr || AbilityCommand == EFHS_AbilityCommand::None || LinkedGA == nullptr)
	{
		return;
	}

	const FGameplayTagContainer* CooldownTags = LinkedGA->GetCooldownTags();
	if (CooldownTags == nullptr)
	{
		return;
	}

	for (auto CooldownTag = CooldownTags->CreateConstIterator(); CooldownTag; ++CooldownTag)
	{
		ASC->RegisterGameplayTagEvent(*CooldownTag, EGameplayTagEventType::AnyCountChange).RemoveAll(this);
	}

	LinkedGA = nullptr;
	
} // CleanFromGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

