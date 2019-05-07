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
#include "StoneAgeColonyCharacter.h"
#include "SurvivalGameState.h"

GameLoader::GameLoader()
{
}

GameLoader::~GameLoader()
{
}

void GameLoader::LoadGame(APawn* InstigatorPawn)
{
	/*
		This method handles everything about loading game from a savefile.
	*/

	// LOAD SYSTEM
	UGameSaver* GameLoader = Cast<UGameSaver>(UGameplayStatics::CreateSaveGameObject(UGameSaver::StaticClass()));
	GameLoader = Cast<UGameSaver>(UGameplayStatics::LoadGameFromSlot(GameLoader->SaveSlotName, GameLoader->UserIndex));

	if (GameLoader) {
		// Destroy existing characters.
		DestroyActors<AEnemyCharacter>();

		// Set varibles to communicator (update with loaded variables).
		Communicator::GetInstance().test = GameLoader->test;
		Communicator::GetInstance().SpawnedCharacterDetails = GameLoader->SpawnedCharacterDetails;
		Communicator::GetInstance().PlayerTransform = GameLoader->PlayerTransform;
		Communicator::GetInstance().PlayerRotation = GameLoader->PlayerRotation;
		Communicator::GetInstance().PlayerHealth = GameLoader->PlayerHealth;
		Communicator::GetInstance().ElapsedGameMinutes = GameLoader->ElapsedGameMinutes;

		// Load player variables.
		((AStoneAgeColonyCharacter*)InstigatorPawn)->SetActorTransform(GameLoader->PlayerTransform);
		((AStoneAgeColonyCharacter*)InstigatorPawn)->SetActorRotation(GameLoader->PlayerRotation);
		((AStoneAgeColonyCharacter*)InstigatorPawn)->Health = Communicator::GetInstance().PlayerHealth;

		ASurvivalGameState* CurrentGameState = Cast<ASurvivalGameState>(Communicator::GetInstance().World->GetGameState());
		CurrentGameState->ElapsedGameMinutes = Communicator::GetInstance().ElapsedGameMinutes;

		// Spawn saved characters.
		SpawnCharacters();
	}

	UE_LOG(LogTemp, Warning, TEXT("GameLoader: Game loaded."));
}

void GameLoader::SpawnCharacters() 
{
	/*
	Spawn previously saved characters from savefile.
	*/

	FActorSpawnParameters SpawnParams;
	//SpawnParams.Owner = this;

	// Get actor details to spawn from communicator.
	TArray<FEnemyCharacterDetails> ActorDetailsToSpawn = Communicator::GetInstance().SpawnedCharacterDetails;
	auto ActorToSpawn = Communicator::GetInstance().EnemyCharacterToSpawn;

	UE_LOG(LogTemp, Warning, TEXT("ActorDetailsToSpawn lenght: %d"), ActorDetailsToSpawn.Num());
	// Iterate over array and saved spawn actors.
	for (FEnemyCharacterDetails Details : ActorDetailsToSpawn)
	{
		FTransform ActorTransform = Details.CharacterTransform;
		AEnemyCharacter* SpawnedActor = Communicator::GetInstance().World->SpawnActor<AEnemyCharacter>(ActorToSpawn, ActorTransform, SpawnParams);
	}

}

template <typename T>
void GameLoader::DestroyActors() 
{
	/*
	Actors are needed to be destroyed first otherwise we would have duplicates.
	This method fixes this problem.
	*/

	// We will reset spawned character details and update it with current details.
	UWorld* YourGameWorld = Communicator::GetInstance().World;

	for (TObjectIterator<T> Itr; Itr; ++Itr)
	{
		// Filter out objects not contained in the target world.
		if (Itr->GetWorld() != YourGameWorld)
		{
			continue;
		}
		if (std::is_same_v<T, AEnemyCharacter>)
		{
			// Register details to communicator.
			Itr->Destroy();
		}

	}

	// Empty communicator since we deleted all characters.
	Communicator::GetInstance().SpawnedCharacterDetails.Empty();
}