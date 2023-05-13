#pragma once

#include "GameFramework/PlayerController.h"
#include "FHS_PlayerController.generated.h"

UCLASS()
class AFHS_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void OnPossess(APawn* P) override;
	
}; // AFHS_PlayerController
