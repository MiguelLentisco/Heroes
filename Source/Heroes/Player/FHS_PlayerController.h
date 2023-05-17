#pragma once

#include "GameFramework/PlayerController.h"
#include "FHS_PlayerController.generated.h"

UCLASS()
class HEROES_API AFHS_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	

protected:
	void CreateHeroSelector();
	
}; // AFHS_PlayerController
