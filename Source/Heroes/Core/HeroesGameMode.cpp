// Copyright Epic Games, Inc. All Rights Reserved.

#include "HeroesGameMode.h"
#include "Heroes/Hero/FHS_BaseHero.h"
#include "UObject/ConstructorHelpers.h"

// ---------------------------------------------------------------------------------------------------------------------

AHeroesGameMode::AHeroesGameMode() : Super()
{
	DefaultPawnClass = AFHS_BaseHero::StaticClass();

} // AHeroesGameMode

// ---------------------------------------------------------------------------------------------------------------------

