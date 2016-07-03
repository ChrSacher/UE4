// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "StalkerGhosts.h"
#include "StalkerGhostsGameMode.h"
#include "StalkerGhostsHUD.h"
#include "StalkerGhostsCharacter.h"

AStalkerGhostsGameMode::AStalkerGhostsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AStalkerGhostsHUD::StaticClass();
}
