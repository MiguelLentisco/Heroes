#include "FHS_UW_HUD.h"

#include "FHS_UW_Abilities.h"
#include "FHS_UW_Attributes.h"
#include "FHS_UW_UltimateAbility.h"
#include "FHS_UW_Weapon.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/Player/FHS_PlayerState.h"

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_HUD::RegisterCallbacks()
{
	GetWorld()->GetTimerManager().SetTimer(SetCallbacksTimer, this, &UFHS_UW_HUD::TryBindCallbacks, 0.1f, true);
	TryBindCallbacks();
	
} // Initialize

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_HUD::BeginDestroy()
{
	if (auto* PS = GetOwningPlayerState<AFHS_PlayerState>())
	{
		PS->OnNumDeathsChanged.RemoveAll(this);
		PS->OnNumKillsChanged.RemoveAll(this);
	}
	
	Super::BeginDestroy();
	
} // BeginDestroy

// ---------------------------------------------------------------------------------------------------------------------


void UFHS_UW_HUD::OnHeroInputChangedInput(UAbilitySystemComponent* ASC, bool bSet)
{
	if (ASC == nullptr)
	{
		return;
	}

	if (bSet)
	{
		ASC->RegisterGameplayTagEvent(TAG_Status_Dead, EGameplayTagEventType::AnyCountChange).AddUObject(
			this, &UFHS_UW_HUD::OnDeadChanged);
		ASC->RegisterGameplayTagEvent(TAG_Status_Stunned, EGameplayTagEventType::AnyCountChange).AddUObject(
			this, &UFHS_UW_HUD::OnStunChanged);
		IFHS_GASListener::Execute_SetupWithGAS(AttributesHUD, ASC);
		IFHS_GASListener::Execute_SetupWithGAS(AbilitiesHUD, ASC);
		IFHS_GASListener::Execute_SetupWithGAS(UltimateAbilityHUD, ASC);
	}
	else
	{
		ASC->RegisterGameplayTagEvent(TAG_Status_Dead, EGameplayTagEventType::AnyCountChange).RemoveAll(this);
		ASC->RegisterGameplayTagEvent(TAG_Status_Stunned, EGameplayTagEventType::AnyCountChange).RemoveAll(this);
		IFHS_GASListener::Execute_CleanFromGAS(AttributesHUD, ASC);
		IFHS_GASListener::Execute_CleanFromGAS(AbilitiesHUD, ASC);
		IFHS_GASListener::Execute_CleanFromGAS(UltimateAbilityHUD, ASC);
	}
	
} // OnHeroInputChangedInput

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_HUD::OnHeroWeaponInputChangedInput(UAbilitySystemComponent* ASC, bool bSet)
{
	if (ASC == nullptr)
	{
		return;
	}

	if (bSet)
	{
		IFHS_GASListener::Execute_SetupWithGAS(WeaponHUD, ASC);
	}
	else
	{
		IFHS_GASListener::Execute_CleanFromGAS(WeaponHUD, ASC);
	}
	
} // OnHeroWeaponInputChangedInput

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_HUD::TryBindCallbacks()
{
	auto* PS = GetOwningPlayerState<AFHS_PlayerState>();
	if (PS == nullptr)
	{
		return;
	}
	
	PS->OnNumDeathsChanged.AddUObject(this, &UFHS_UW_HUD::SetNumDeaths);
	PS->OnNumKillsChanged.AddUObject(this, &UFHS_UW_HUD::SetNumKills);
	GetWorld()->GetTimerManager().ClearTimer(SetCallbacksTimer);
	
} // TryBindCallbacks

// ---------------------------------------------------------------------------------------------------------------------

