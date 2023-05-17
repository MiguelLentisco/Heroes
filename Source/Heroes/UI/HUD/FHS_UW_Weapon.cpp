#include "FHS_UW_Weapon.h"

#include "Heroes/GAS/FHS_AbilityCommand.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Abilities/FHS_GA_Reload.h"
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

	ASC->RegisterGameplayTagEvent(TAG_Status_Reloading, EGameplayTagEventType::AnyCountChange).AddUObject(
		this, &UFHS_UW_Weapon::OnReloadingChanged, ASC);
	
	
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
	ASC->RegisterGameplayTagEvent(TAG_Status_Reloading, EGameplayTagEventType::AnyCountChange).RemoveAll(this);
	
} // CleanFromGAS_Implementation

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_Weapon::OnReloadingChanged(const FGameplayTag PoisonTag, int32 TagCount, UAbilitySystemComponent* ASC)
{
	if (TagCount == 0)
	{
		SetReloadingStatus(false, 0.f);
		return;
	}
	
	if (ASC == nullptr)
	{
		return;
	}

	constexpr int32 ReloadCommandID = static_cast<int32>(EFHS_AbilityCommand::Ability_Reload);
	const FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromInputID(ReloadCommandID);
	if (AbilitySpec == nullptr)
	{
		return;
	}

	TArray<UGameplayAbility*> GAs = AbilitySpec->GetAbilityInstances();
	if (GAs.IsEmpty())
	{
		return;
	}

	auto* GA = Cast<UFHS_GA_Reload>(GAs[0]);
	if (GA == nullptr)
	{
		return;
	}

	SetReloadingStatus(true, GA->ReloadTime.GetValueAtLevel(GA->GetAbilityLevel()));
	
} // OnReloadingChanged

// ---------------------------------------------------------------------------------------------------------------------

