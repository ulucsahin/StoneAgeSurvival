// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLoader.h"
#include "GameSaver.h"
#include "PeopleSpawner.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Communicator.h"
#include "EnemyCharacter.h"
#include "PeopleSpawner.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "MyPawn.h"

GameLoader::GameLoader()
{
	//static ConstructorHelpers::FClassFinder<AEnemyCharacter> BPClassToSpawn(TEXT("Blueprint'/Game/Uluc/ActiveAssets/BP_FollowerEnemyCharacter_C'"));
	//static ConstructorHelpers::FClassFinder<APawn> BPClass(TEXT("'/Game/BP_MyPawn'"));
	//static ConstructorHelpers::FClassFinder<UBlueprint> BP_FollowerEnemyCharacter(TEXT("Blueprint'/Game/Uluc/ActiveAssets/BP_FollowerEnemyCharacter_C'"));
	UE_LOG(LogTemp, Warning, TEXT("ffs..."));
}

GameLoader::~GameLoader()
{
}


void GameLoader::LoadGame() 
{
	// LOAD SYSTEM
	UGameSaver* GameLoader = Cast<UGameSaver>(UGameplayStatics::CreateSaveGameObject(UGameSaver::StaticClass()));
	GameLoader = Cast<UGameSaver>(UGameplayStatics::LoadGameFromSlot(GameLoader->SaveSlotName, GameLoader->UserIndex));

	if (GameLoader) {

		// Teleport player to saved location.
		// not implemented

		// Set varibles to communicator.
		Communicator::GetInstance().test = GameLoader->test;
		Communicator::GetInstance().SpawnedCharacterDetails = GameLoader->SpawnedCharacterDetails;

		// Spawn saved characters.
		SpawnCharacters();
	}

	UE_LOG(LogTemp, Warning, TEXT("Game loaded."));
}

void GameLoader::SpawnCharacters() 
{
	FActorSpawnParameters SpawnParams;
	//SpawnParams.Owner = this;

	// Get actor details to spawn from communicator.
	TArray<FEnemyCharacterDetails> ActorDetailsToSpawn = Communicator::GetInstance().SpawnedCharacterDetails;
	auto ActorToSpawn = Communicator::GetInstance().EnemyCharacterToSpawn;

	UE_LOG(LogTemp, Warning, TEXT("ActorDetailsToSpawn lenght: %d"), ActorDetailsToSpawn.Num());
	// Iterate over array and saved spawn actors.
	for (FEnemyCharacterDetails Details : ActorDetailsToSpawn)
	{
		FTransform CurrentTransform;
		CurrentTransform.SetLocation(Details.CharacterLocation);
		AEnemyCharacter* SpawnedActor = Communicator::GetInstance().World->SpawnActor<AEnemyCharacter>(ActorToSpawn, CurrentTransform, SpawnParams);
	}

}