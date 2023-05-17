#include "FHS_UW_PauseMenu.h"

#include "Kismet/GameplayStatics.h"

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_PauseMenu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetPlayerContext().GetPlayerController(), EQuitPreference::Quit, true);

} // QuitGame

// ---------------------------------------------------------------------------------------------------------------------