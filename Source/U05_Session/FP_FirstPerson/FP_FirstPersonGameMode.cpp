// Copyright Epic Games, Inc. All Rights Reserved.

#include "FP_FirstPersonGameMode.h"
#include "FP_FirstPersonHUD.h"
#include "FP_FirstPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Game/CGameState.h"

AFP_FirstPersonGameMode::AFP_FirstPersonGameMode()
{
	// Default Pawn Class
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// HUD Class
	HUDClass = AFP_FirstPersonHUD::StaticClass();

	// GameState Class
	GameStateClass = ACGameState::StaticClass();
}
