#include "FHS_PlayerState.h"

#include "Net/UnrealNetwork.h"

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_PlayerState::AddKill()
{
	++NumKills;
	OnRep_NumKills();
	
} // AddKill

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_PlayerState::AddDeath()
{
	++NumDeaths;
	OnRep_NumDeaths();
	
} // AddDeath

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AFHS_PlayerState, NumKills, COND_None);
	DOREPLIFETIME_CONDITION(AFHS_PlayerState, NumDeaths, COND_None);

} // GetLifetimeReplicatedProps

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_PlayerState::OnRep_NumKills()
{
	OnNumKillsChanged.Broadcast(NumKills);
	
} // OnRep_NumKills

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_PlayerState::OnRep_NumDeaths()
{
	OnNumDeathsChanged.Broadcast(NumDeaths);
	
} // OnRep_NumDeaths

// ---------------------------------------------------------------------------------------------------------------------

