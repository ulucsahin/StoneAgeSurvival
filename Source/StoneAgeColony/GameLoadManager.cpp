// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLoadManager.h"
#include "SaveGameEntity.h"
#include "PeopleSpawner.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Communicator.h"
#include "EnemyCharacter.h"
#include "GatherableTree.h"
#include "Building.h"
#include "PeopleSpawner.h"
#include "UIBottomBar.h"
#include "UIPlayerInventory.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "StoneAgeColonyCharacter.h"
#include "SurvivalGameState.h"

// Sets default values
AGameLoadManager::AGameLoadManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameLoadManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameLoadManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AGameLoadManager::LoadGame(APawn* InstigatorPawn)
{
	/* This method handles everything about loading game from a savefile. */
	UE_LOG(LogTemp, Warning, TEXT("GameLoader 1"));
	// LOAD SYSTEM
	USaveGameEntity* SaveGameEntityLoad = Cast<USaveGameEntity>(UGameplayStatics::CreateSaveGameObject(USaveGameEntity::StaticClass()));
	SaveGameEntityLoad = Cast<USaveGameEntity>(UGameplayStatics::LoadGameFromSlot(SaveGameEntityLoad->SaveSlotName, SaveGameEntityLoad->UserIndex));
	UE_LOG(LogTemp, Warning, TEXT("GameLoader 2"));
	if (SaveGameEntityLoad)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameLoader 3"));
		// Destroy existing characters that should be deleted before loading.
		DestroyActors<AEnemyCharacter>();
		DestroyActors<AGatherableTree>();
		DestroyActors<ABuilding>();
		UE_LOG(LogTemp, Warning, TEXT("GameLoader 4"));
		// Load varibles to communicator (update with loaded variables).
		Communicator::GetInstance().test = SaveGameEntityLoad->test;
		Communicator::GetInstance().SpawnedCharacterDetails = SaveGameEntityLoad->SpawnedCharacterDetails;
		Communicator::GetInstance().SpawnedGatherableTreeDetails = SaveGameEntityLoad->SpawnedGatherableTreeDetails;
		Communicator::GetInstance().SpawnedBuildingDetails = SaveGameEntityLoad->SpawnedBuildingDetails;
		Communicator::GetInstance().PlayerTransform = SaveGameEntityLoad->PlayerTransform;
		Communicator::GetInstance().PlayerRotation = SaveGameEntityLoad->PlayerRotation;
		Communicator::GetInstance().PlayerHealth = SaveGameEntityLoad->PlayerHealth;
		Communicator::GetInstance().PlayerLevel = SaveGameEntityLoad->PlayerLevel;
		Communicator::GetInstance().PlayerExperience = SaveGameEntityLoad->PlayerExperience;
		Communicator::GetInstance().PlayerGold = SaveGameEntityLoad->PlayerGold;
		Communicator::GetInstance().ElapsedGameMinutes = SaveGameEntityLoad->ElapsedGameMinutes;
		UE_LOG(LogTemp, Warning, TEXT("GameLoader 5"));
		// Load player variables.
		((AStoneAgeColonyCharacter*)InstigatorPawn)->SetActorTransform(SaveGameEntityLoad->PlayerTransform);
		((AStoneAgeColonyCharacter*)InstigatorPawn)->SetActorRotation(SaveGameEntityLoad->PlayerRotation);
		((AStoneAgeColonyCharacter*)InstigatorPawn)->Health = Communicator::GetInstance().PlayerHealth;
		((AStoneAgeColonyCharacter*)InstigatorPawn)->Level = Communicator::GetInstance().PlayerLevel;
		((AStoneAgeColonyCharacter*)InstigatorPawn)->Experience = Communicator::GetInstance().PlayerExperience;
		((AStoneAgeColonyCharacter*)InstigatorPawn)->Gold = Communicator::GetInstance().PlayerGold;
		((AStoneAgeColonyCharacter*)InstigatorPawn)->Inventory = SaveGameEntityLoad->PlayerInventory;
		UE_LOG(LogTemp, Warning, TEXT("GameLoader 6"));
		ASurvivalGameState* CurrentGameState = Cast<ASurvivalGameState>(Communicator::GetInstance().World->GetGameState());
		CurrentGameState->ElapsedGameMinutes = Communicator::GetInstance().ElapsedGameMinutes;
		UE_LOG(LogTemp, Warning, TEXT("GameLoader 7"));
		// Update UI Inventory Elements
		RefreshUI(InstigatorPawn);
		UE_LOG(LogTemp, Warning, TEXT("GameLoader 8"));
		// Spawn saved characters.
		SpawnLoadedActors<AEnemyCharacter>();
		SpawnLoadedActors<AGatherableTree>();
		SpawnLoadedActors<ABuilding>();
		UE_LOG(LogTemp, Warning, TEXT("GameLoader 9"));
	}

	UE_LOG(LogTemp, Warning, TEXT("GameLoader: Game loaded."));
}


template <typename T>
void AGameLoadManager::SpawnLoadedActors()
{
	/* Spawn previously saved characters from savefile. */
	T::SpawnLoadedActors();
}


template <typename T>
void AGameLoadManager::DestroyActors()
{
	/* Actors are needed to be destroyed first otherwise we would have duplicates. */

	// We will reset spawned character details and update it with current details.
	UWorld* YourGameWorld = Communicator::GetInstance().World;

	for (TObjectIterator<T> Itr; Itr; ++Itr)
	{
		// Filter out objects not contained in the target world.
		if (Itr->GetWorld() != YourGameWorld)
		{
			continue;
		}
		else
		{
			Itr->Destroy();
		}

	}

	// Empty communicator since we deleted all characters.
	T::EmptyCommunicatorDetailsArray();
}


void AGameLoadManager::RefreshUI(APawn* InstigatorPawn)
{
	auto UIBottomBar = ((AStoneAgeColonyCharacter*)InstigatorPawn)->BottomBar;
	auto UIPlayerInventory = ((AStoneAgeColonyCharacter*)InstigatorPawn)->UIPlayerInventory;

	if (UIBottomBar)
	{
		UIBottomBar->Refresh();
	}

	if (UIPlayerInventory)
	{
		UIPlayerInventory->Refresh();
	}
}