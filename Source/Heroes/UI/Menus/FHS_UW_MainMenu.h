#pragma once

#include "Blueprint/UserWidget.h"
#include "FHS_UW_MainMenu.generated.h"

UCLASS()
class HEROES_API UFHS_UW_MainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void PlaySinglePlayer();
	
	UFUNCTION(BlueprintCallable)
	void OpenLevelListening();

	UFUNCTION(BlueprintCallable)
	void JoinWithIP(FName IP);
	
	UFUNCTION(BlueprintCallable)
	void QuitGame();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> Level;
	
}; // UFHS_UW_MainMenu


