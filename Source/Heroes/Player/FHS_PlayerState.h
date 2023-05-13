#pragma once

#include "GameFramework/PlayerState.h"
#include "FHS_PlayerState.generated.h"

class UFHS_HeroData;

UCLASS()
class HEROES_API AFHS_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UFHS_HeroData> ChosenHeroData;
	
}; // AFHS_PlayerState
