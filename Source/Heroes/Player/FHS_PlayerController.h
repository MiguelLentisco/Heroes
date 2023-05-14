#pragma once

#include "GameFramework/PlayerController.h"
#include "FHS_PlayerController.generated.h"

UCLASS()
class AFHS_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void AcknowledgePossession(APawn* P) override;

protected:
	void SetupHUD(APawn* P);
	
}; // AFHS_PlayerController
