#include "FHS_Attributes_CharacterCore.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

// ---------------------------------------------------------------------------------------------------------------------

// From https://github.com/tranek/GASDocumentation#concepts-as-attributes
void UFHS_Attributes_CharacterCore::OnRep_CurrentHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFHS_Attributes_CharacterCore, CurrentHealth, OldValue);
	
} // OnRep_CurrentHealth

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_Attributes_CharacterCore::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFHS_Attributes_CharacterCore, MaxHealth, OldValue);
	
} // OnRep_MaxHealth

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_Attributes_CharacterCore::OnRep_CurrentUltPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFHS_Attributes_CharacterCore, CurrentUltimatePower, OldValue);
	
} // OnRep_CurrentUltPower

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_Attributes_CharacterCore::OnRep_MaxUltPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFHS_Attributes_CharacterCore, MaxUltimatePower, OldValue);
	
} // OnRep_MaxtUltPower

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_Attributes_CharacterCore::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UFHS_Attributes_CharacterCore, CurrentHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFHS_Attributes_CharacterCore, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFHS_Attributes_CharacterCore, CurrentUltimatePower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFHS_Attributes_CharacterCore, MaxUltimatePower, COND_None, REPNOTIFY_Always);
	
} // GetLifetimeReplicatedProps

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_Attributes_CharacterCore::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	ClampAttributes(Data.EvaluatedData.Attribute);
	
} // PostGameplayEffectExecute

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_Attributes_CharacterCore::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue,
	float NewValue)
{
	ClampAttributes(Attribute);
	
} // PostAttributeChange

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_Attributes_CharacterCore::ClampAttributes(const FGameplayAttribute& Attribute)
{
	if (Attribute == GetCurrentHealthAttribute())
	{
		CurrentHealth.SetBaseValue(FMath::Clamp(CurrentHealth.GetBaseValue(), 0.f, MaxHealth.GetBaseValue()));
	}
	else if (Attribute == GetCurrentUltimatePowerAttribute())
	{
		CurrentUltimatePower.SetBaseValue(FMath::Clamp(CurrentUltimatePower.GetBaseValue(), 0.f,
													   MaxUltimatePower.GetBaseValue()));
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		MaxHealth.SetBaseValue(FMath::Max(0.f, MaxHealth.GetBaseValue()));
	}
	else if (Attribute == GetMaxUltimatePowerAttribute())
	{
		MaxUltimatePower.SetBaseValue(FMath::Max(0.f, MaxUltimatePower.GetBaseValue()));
	}
	
} // ClampAttributes

// ---------------------------------------------------------------------------------------------------------------------
