// Fill out your copyright notice in the Description page of Project Settings.

#include "TestGameLoader.h"
#include "GameSaver.h"
#include "GameLoader.h"
#include "PeopleSpawner.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Communicator.h"
#include "EnemyCharacter.h"


void ATestGameLoader::OnUsed(APawn* InstigatorPawn)
{
	// LOAD SYSTEM
	// We will retrieve saved variables from GameSaver object.
	UGameSaver* GameSaver = Cast<UGameSaver>(UGameplayStatics::CreateSaveGameObject(UGameSaver::StaticClass()));
	GameSaver = Cast<UGameSaver>(UGameplayStatics::LoadGameFromSlot(GameSaver->SaveSlotName, GameSaver->UserIndex));

	GameLoader* GameLoaderInstance = new GameLoader();
	if (GameSaver) 
	{
		// Teleport player to saved location.
		InstigatorPawn->SetActorLocation(GameSaver->PlayerLocation);
		GameLoaderInstance->LoadGame();
	}

	//static ConstructorHelpers::FObjectFinder<AEnemyCharacter> BPClassToSpawnTest(TEXT("/Game/TEST"));

	UE_LOG(LogTemp, Warning, TEXT("Game loaded."));
}
