#include "FHS_UW_Weapon.h"

#include "Heroes/GAS/Attributes/FHS_Attributes_Weapon.h"

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_Weapon::SetupWithGAS_Implementation(UAbilitySystemComponent* ASC)
{
	if (ASC == nullptr)
	{
		return;
	}
	
	const FGameplayAttribute CurrentAmmoAttribute = UFHS_Attributes_Weapon::GetCurrentAmmoAttribute();
	
	ASC->GetGameplayAttributeValueChangeDelegate(CurrentAmmoAttribute).AddWeakLambda(this,
	[this](const FOnAttributeChangeData& ChangedData)
	{
		OnCurrentAmmoChanged(ChangedData.Attribute, ChangedData.NewValue, ChangedData.OldValue);
	});

	AmmoClip = 0;
	CurrentAmmo = 0;

	const FGameplayAttribute AmmoClipAttribute = UFHS_Attributes_Weapon::GetAmmoClipAttribute();
	AmmoClip = ASC->GetNumericAttribute(AmmoClipAttribute);
	CurrentAmmo = ASC->GetNumericAttribute(CurrentAmmoAttribute);
	
	SetupReady();
	
} // SetupWithGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_Weapon::CleanFromGAS_Implementation(UAbilitySystemComponent* ASC)
{
	if (ASC == nullptr)
	{
		return;
	}
	
	const FGameplayAttribute CurrentAmmoAttribute = UFHS_Attributes_Weapon::GetCurrentAmmoAttribute();
	ASC->GetGameplayAttributeValueChangeDelegate(CurrentAmmoAttribute).RemoveAll(this);
	
} // CleanFromGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

