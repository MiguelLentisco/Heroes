#pragma once

#include "Blueprint/UserWidget.h"
#include "FHS_UW_PauseMenu.generated.h"

UCLASS()
class HEROES_API UFHS_UW_PauseMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void QuitGame();
	
}; // UFHS_UW_PauseMenu
