// Copyright Epic Games, Inc. All Rights Reserved.

#include "HeroesGameMode.h"
#include "Heroes/Character/FHS_BaseCharacter.h"
#include "UObject/ConstructorHelpers.h"

// ---------------------------------------------------------------------------------------------------------------------

AHeroesGameMode::AHeroesGameMode() : Super()
{
	DefaultPawnClass = AFHS_BaseCharacter::StaticClass();

} // AHeroesGameMode

// ---------------------------------------------------------------------------------------------------------------------

