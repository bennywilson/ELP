// Copyright Epic Games, Inc. All Rights Reserved.

#include "OxiGameMode.h"
#include "OxiHUD.h"
#include "OxiCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOxiGameMode::AOxiGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AOxiHUD::StaticClass();
}
