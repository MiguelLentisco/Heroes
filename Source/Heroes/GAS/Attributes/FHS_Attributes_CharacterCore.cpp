#include "FHS_Attributes_CharacterCore.h"

#include "GameplayEffectExtension.h"
#include "Heroes/Hero/FHS_BaseHero.h"
#include "Net/UnrealNetwork.h"

// ---------------------------------------------------------------------------------------------------------------------

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

void UFHS_Attributes_CharacterCore::OnRep_Speed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFHS_Attributes_CharacterCore, Speed, OldValue);
	
} // OnRep_Speed

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_Attributes_CharacterCore::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UFHS_Attributes_CharacterCore, CurrentHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFHS_Attributes_CharacterCore, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFHS_Attributes_CharacterCore, CurrentUltimatePower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFHS_Attributes_CharacterCore, MaxUltimatePower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFHS_Attributes_CharacterCore, Speed, COND_None, REPNOTIFY_Always);
	
} // GetLifetimeReplicatedProps

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_Attributes_CharacterCore::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetCurrentHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, MaxHealth.GetCurrentValue());
	}
	else if (Attribute == GetCurrentUltimatePowerAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, MaxUltimatePower.GetCurrentValue());
	}
	
} // PreAttributeChange

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_Attributes_CharacterCore::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	ClampAttributes(Data.EvaluatedData.Attribute);
	
} // PostGameplayEffectExecute

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_Attributes_CharacterCore::ClampAttributes(const FGameplayAttribute& Attribute)
{
	if (Attribute == GetCurrentHealthAttribute())
	{
		CurrentHealth.SetBaseValue(FMath::Clamp(CurrentHealth.GetBaseValue(), 0.f, MaxHealth.GetCurrentValue()));
	}
	else if (Attribute == GetCurrentUltimatePowerAttribute())
	{
		CurrentUltimatePower.SetBaseValue(FMath::Clamp(CurrentUltimatePower.GetBaseValue(), 0.f,
													   MaxUltimatePower.GetCurrentValue()));
	}
	
} // ClampAttributes

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_Attributes_CharacterCore::InitFromMetaDataTable(const UDataTable* DataTable)
{
	const float OldCurrentHealth = CurrentHealth.GetBaseValue();
	const float OldMaxHealth = MaxHealth.GetBaseValue();
	float OldCurrentUltPower = CurrentUltimatePower.GetBaseValue();
	const float OldMaxUltPower = MaxUltimatePower.GetBaseValue();
	const float OldSpeed = Speed.GetBaseValue();
	
	Super::InitFromMetaDataTable(DataTable);

	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	if (ASC == nullptr)
	{
		return;
	}

	// Don't erase ulti power
	if (const auto* Hero = Cast<AFHS_BaseHero>(ASC->GetOwner()))
	{
		if (Hero->IsInitStatsDueDeath())
		{
			CurrentUltimatePower.SetBaseValue(OldCurrentUltPower);
		}
	}

	OnRep_CurrentHealth(OldCurrentHealth);
	OnRep_MaxHealth(OldMaxHealth);
	OnRep_CurrentUltPower(OldCurrentUltPower);
	OnRep_MaxUltPower(OldMaxUltPower);
	OnRep_Speed(OldSpeed);
	
} // InitFromMetaDataTable

// ---------------------------------------------------------------------------------------------------------------------
