#pragma once

#include "GameFramework/PlayerState.h"
#include "FHS_PlayerState.generated.h"

class UFHS_HeroData;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerScoreChanged, int32)

UCLASS()
class HEROES_API AFHS_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	FOnPlayerScoreChanged OnNumKillsChanged;
	FOnPlayerScoreChanged OnNumDeathsChanged;

	void AddKill();
	void AddDeath();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_NumKills();
	UFUNCTION()
	void OnRep_NumDeaths();

protected:
	UPROPERTY(Replicated, ReplicatedUsing = "OnRep_NumKills")
	int32 NumKills = 0;

	UPROPERTY(Replicated, ReplicatedUsing = "OnRep_NumDeaths")
	int32 NumDeaths = 0;
	
}; // AFHS_PlayerState
