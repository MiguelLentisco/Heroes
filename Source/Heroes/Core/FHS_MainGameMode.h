#pragma once

#include "FHS_BaseGameMode.h"
#include "FHS_MainGameMode.generated.h"

UCLASS()
class HEROES_API AFHS_MainGameMode : public AFHS_BaseGameMode
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float RespawnTime = 4.f;

	void OnPlayerDead(APlayerController* PC);
	void RespawnPlayerDead(APlayerController* PC);

	
}; // AFHS_MainGameMode
