#include "FHS_UW_Attributes.h"

#include "AbilitySystemComponent.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_Attributes::SetupWithGAS_Implementation(UAbilitySystemComponent* ASC)
{
	CurrentASC = ASC;

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
	
} // SetupWithGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_Attributes::CleanFromGAS_Implementation(UAbilitySystemComponent* ASC)
{
	if (CurrentASC == nullptr)
	{
		return;
	}

	CurrentASC->GetGameplayAttributeValueChangeDelegate(UFHS_Attributes_CharacterCore::GetCurrentHealthAttribute()).
				RemoveAll(this);
	CurrentASC->GetGameplayAttributeValueChangeDelegate(UFHS_Attributes_CharacterCore::GetCurrentUltimatePowerAttribute()).
				RemoveAll(this);
	CurrentASC = nullptr;
	
} // CleanFromGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_Attributes::BeginDestroy()
{
	CleanFromGAS_Implementation(nullptr);
	
	Super::BeginDestroy();
	
} // BeginDestroy

// ---------------------------------------------------------------------------------------------------------------------

