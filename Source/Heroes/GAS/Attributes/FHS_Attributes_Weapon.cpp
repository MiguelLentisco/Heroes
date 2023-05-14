#include "FHS_Attributes_Weapon.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

// ---------------------------------------------------------------------------------------------------------------------

// From https://github.com/tranek/GASDocumentation#concepts-as-attributes
void UFHS_Attributes_Weapon::OnRep_CurrentAmmo(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFHS_Attributes_Weapon, CurrentAmmo, OldValue);
	
} // OnRep_CurrentHealth

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_Attributes_Weapon::OnRep_AmmoClip(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFHS_Attributes_Weapon, AmmoClip, OldValue);
	
} // OnRep_MaxHealth

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_Attributes_Weapon::OnRep_FireRate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFHS_Attributes_Weapon, FireRate, OldValue);
	
} // OnRep_CurrentUltPower

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_Attributes_Weapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UFHS_Attributes_Weapon, CurrentAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFHS_Attributes_Weapon, AmmoClip, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFHS_Attributes_Weapon, FireRate, COND_None, REPNOTIFY_Always);
	
} // GetLifetimeReplicatedProps

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_Attributes_Weapon::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	const FGameplayAttribute& Attribute = Data.EvaluatedData.Attribute;

	if (Attribute == GetCurrentAmmoAttribute())
	{
		CurrentAmmo.SetBaseValue(FMath::Clamp(CurrentAmmo.GetBaseValue(), 0.f, AmmoClip.GetBaseValue()));
	}
	else if (Attribute == GetAmmoClipAttribute())
	{
		AmmoClip.SetBaseValue(FMath::Max(0.f, AmmoClip.GetBaseValue()));
	}
	else if (Attribute == GetFireRateAttribute())
	{
		FireRate.SetBaseValue(FMath::Max(0.f, FireRate.GetBaseValue()));	
	}
	
} // PostGameplayEffectExecute

// ---------------------------------------------------------------------------------------------------------------------
