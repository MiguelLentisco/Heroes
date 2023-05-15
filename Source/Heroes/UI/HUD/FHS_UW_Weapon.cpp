#include "FHS_UW_Weapon.h"

#include "Heroes/GAS/Attributes/FHS_Attributes_Weapon.h"

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_Weapon::SetupWithGAS_Implementation(UAbilitySystemComponent* ASC,
                                                 const TArray<UAbilitySystemComponent*>& WeaponASCs)
{
	const FGameplayAttribute CurrentAmmoAttribute = UFHS_Attributes_Weapon::GetCurrentAmmoAttribute();
	for (int32 i = 0; i < WeaponASCs.Num(); ++i)
	{
		WeaponASCs[i]->GetGameplayAttributeValueChangeDelegate(CurrentAmmoAttribute).AddWeakLambda(this,
		[this, i](const FOnAttributeChangeData& ChangedData)
		{
			if (i == CurrentWeaponID)
			{
				OnCurrentAmmoChanged(ChangedData.Attribute, ChangedData.NewValue, ChangedData.OldValue);
			}
		});
	}

	const FGameplayAttribute AmmoClipAttribute = UFHS_Attributes_Weapon::GetAmmoClipAttribute();
	const bool bHasCurrentWeapon = WeaponASCs.IsValidIndex(CurrentWeaponID);
	AmmoClip = bHasCurrentWeapon ? FMath::Floor(WeaponASCs[CurrentWeaponID]->GetNumericAttribute(AmmoClipAttribute)) : 0;
	SetupReady();
	
} // SetupWithGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_Weapon::CleanFromGAS_Implementation(UAbilitySystemComponent* ASC,
                                                 const TArray<UAbilitySystemComponent*>& WeaponASCs)
{
	const FGameplayAttribute CurrentAmmoAttribute = UFHS_Attributes_Weapon::GetCurrentAmmoAttribute();
	for (int32 i = 0; i < WeaponASCs.Num(); ++i)
	{
		WeaponASCs[i]->GetGameplayAttributeValueChangeDelegate(CurrentAmmoAttribute).RemoveAll(this);
	}
	CurrentWeaponID = 0;
	
} // CleanFromGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

