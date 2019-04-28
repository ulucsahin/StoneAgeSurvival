// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "StoneAgeColonyGameMode.h"
#include "StoneAgeColonyHUD.h"
#include "StoneAgeColonyCharacter.h"
#include "UObject/ConstructorHelpers.h"

AStoneAgeColonyGameMode::AStoneAgeColonyGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AStoneAgeColonyHUD::StaticClass();
}
