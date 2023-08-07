// Copyright Epic Games, Inc. All Rights Reserved.

#include "U05_SessionGameMode.h"
#include "U05_SessionCharacter.h"
#include "UObject/ConstructorHelpers.h"

AU05_SessionGameMode::AU05_SessionGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
