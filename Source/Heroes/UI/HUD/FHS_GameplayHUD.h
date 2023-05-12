#pragma once

#include "GameFramework/HUD.h"
#include "FHS_GameplayHUD.generated.h"

UCLASS()
class HEROES_API AFHS_GameplayHUD : public AHUD
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFHS_GameplayHUD();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
}; // AFHS_GameplayHUD
