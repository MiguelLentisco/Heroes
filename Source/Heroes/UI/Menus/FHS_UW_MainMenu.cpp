#include "FHS_UW_MainMenu.h"

#include "Kismet/GameplayStatics.h"

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_MainMenu::PlaySinglePlayer()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Level);
	
} // PlaySinglePlayer

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_MainMenu::JoinWithIP(FName IP)
{
	UGameplayStatics::OpenLevel(GetWorld(), IP);
	
} // JoinWithIP

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_MainMenu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0),
	                               EQuitPreference::Quit, true);
	
} // QuitGame

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_MainMenu::OpenLevelListening()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Level, true, TEXT("?listen"));
	
} // OpenLevelListening

// ---------------------------------------------------------------------------------------------------------------------

