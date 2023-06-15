#pragma once

#include "GameFramework/PlayerController.h"
#include "FHS_PlayerController.generated.h"

class AFHS_BaseHero;
struct FGameplayTag;

UCLASS()
class HEROES_API AFHS_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AFHS_BaseHero* GetHero() const { return Hero; }
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void AcknowledgePossession(APawn* P) override;

	void OnPlayerDeadChanged(const FGameplayTag DeadTag, int32 NumCount);
	UFUNCTION(Client, Reliable)
	void ChangeCameraPOV(bool bSet1P);

protected:
	UPROPERTY()
	TObjectPtr<AFHS_BaseHero> Hero;
	
}; // AFHS_PlayerController
