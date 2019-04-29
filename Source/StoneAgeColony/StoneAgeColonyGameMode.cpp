// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "StoneAgeColonyGameMode.h"
#include "StoneAgeColonyHUD.h"
#include "StoneAgeColonyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "SurvivalGameState.h"

AStoneAgeColonyGameMode::AStoneAgeColonyGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AStoneAgeColonyHUD::StaticClass();

	/* Assign the class types used by this gamemode */
	//PlayerControllerClass = ASPlayerController::StaticClass();
	//PlayerStateClass = ASPlayerState::StaticClass();
	GameStateClass = ASurvivalGameState::StaticClass();
	//SpectatorClass = ASSpectatorPawn::StaticClass();

	bAllowFriendlyFireDamage = false;
	bSpawnZombiesAtNight = true;

	/* Start the game at 16:00 */
	TimeOfDayStart = 16 * 60;
	BotSpawnInterval = 5.0f;

	/* Default team is 1 for players and 0 for enemies */
	PlayerTeamNum = 1;
}


void AStoneAgeColonyGameMode::InitGameState()
{
	Super::InitGameState();
	UE_LOG(LogTemp, Warning, TEXT("AStoneAgeColonyGameMode::InitGameState()"));
	ASurvivalGameState* MyGameState = Cast<ASurvivalGameState>(GameState);
	if (MyGameState)
	{
		MyGameState->ElapsedGameMinutes = TimeOfDayStart;
	}
}

void AStoneAgeColonyGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	/* Set timer to run every second */
	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AStoneAgeColonyGameMode::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);
}


void AStoneAgeColonyGameMode::StartMatch() { 
	
}

void AStoneAgeColonyGameMode::DefaultTimer() {
	ASurvivalGameState* MyGameState = Cast<ASurvivalGameState>(GameState);
	if (MyGameState)
	{
		/* Increment our time of day */
		MyGameState->ElapsedGameMinutes += MyGameState->GetTimeOfDayIncrement();

		/* Determine our state */
		MyGameState->GetAndUpdateIsNight();

		/* Trigger events when night starts or ends */
		bool CurrentIsNight = MyGameState->GetIsNight();
		if (CurrentIsNight != LastIsNight)
		{
			/* The night just ended, respawn all dead players */
			if (!CurrentIsNight)
			{
				OnNightEnded();
			}
		}
	}
}

void AStoneAgeColonyGameMode::OnNightEnded() {

}