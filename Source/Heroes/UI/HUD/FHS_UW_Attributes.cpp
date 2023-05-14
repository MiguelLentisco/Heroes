#include "FHS_UW_Attributes.h"

#include "AbilitySystemComponent.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_Attributes::SetupWithGAS_Implementation(UAbilitySystemComponent* ASC)
{
	if (ASC == nullptr)
	{
		return;
	}
	
	CurrentASC = ASC;

	// Set Health/Power Values and callbacks
	const FGameplayAttribute HealthAttribute = UFHS_Attributes_CharacterCore::GetCurrentHealthAttribute();
	ASC->GetGameplayAttributeValueChangeDelegate(HealthAttribute).AddWeakLambda(this,
		[this](const FOnAttributeChangeData& ChangedData)
		{
			OnHealthChanged(ChangedData.Attribute, ChangedData.NewValue, ChangedData.OldValue);
		});
	
	const FGameplayAttribute UltimatePowerAttribute = UFHS_Attributes_CharacterCore::GetCurrentUltimatePowerAttribute();
	ASC->GetGameplayAttributeValueChangeDelegate(UltimatePowerAttribute).AddWeakLambda(this,
		[this](const FOnAttributeChangeData& ChangedData)
		{
			OnUltimatePowerChanged(ChangedData.Attribute, ChangedData.NewValue, ChangedData.OldValue);
		});

	MaxHealth = CurrentASC->GetNumericAttribute(UFHS_Attributes_CharacterCore::GetMaxHealthAttribute());
	MaxUltimatePower = CurrentASC->GetNumericAttribute(UFHS_Attributes_CharacterCore::GetMaxUltimatePowerAttribute());

	OnHealthChanged(HealthAttribute, CurrentASC->GetNumericAttribute(HealthAttribute), 0.f);
	OnUltimatePowerChanged(UltimatePowerAttribute, CurrentASC->GetNumericAttribute(UltimatePowerAttribute), 0.f);

	// Callbacks for effects like poison
	ASC->RegisterGameplayTagEvent(TAG_Status_Poison.GetTag(), EGameplayTagEventType::AnyCountChange).AddUObject(
		this, &UFHS_UW_Attributes::OnPoisonEffectChanged);
	
} // SetupWithGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_Attributes::CleanFromGAS_Implementation(UAbilitySystemComponent* ASC)
{
	if (CurrentASC == nullptr)
	{
		return;
	}

	ASC->GetGameplayAttributeValueChangeDelegate(UFHS_Attributes_CharacterCore::GetCurrentHealthAttribute()).
	     RemoveAll(this);
	ASC->GetGameplayAttributeValueChangeDelegate(UFHS_Attributes_CharacterCore::GetCurrentUltimatePowerAttribute()).
	     RemoveAll(this);
	ASC->RegisterGameplayTagEvent(TAG_Status_Poison.GetTag(), EGameplayTagEventType::AnyCountChange).RemoveAll(this);
	
	CurrentASC = nullptr;
	
} // CleanFromGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------