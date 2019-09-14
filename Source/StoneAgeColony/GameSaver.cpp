// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSaver.h"
#include "SaveGameEntity.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "StoneAgeColonyCharacter.h"
#include "EnemyCharacter.h"
#include "Communicator.h"
#include "Building.h"
#include "Structure.h"
#include "Settlement.h"
#include "CraftingStation.h"
#include "House.h"
//#include "Runtime/CoreUObject/Public/UObject/UObjectIterator.h"

GameSaver::GameSaver()
{
}

GameSaver::~GameSaver()
{
}

void GameSaver::SaveGame(APawn* InstigatorPawn)
{
	// Test
	Communicator::GetInstance().Increment();

	// TODO: Make a stats struct for characters.
	// Some variables are only assigned to communicator before saving. Those variables are assigned here.
	USaveGameEntity* SaveGameEntity = Cast<USaveGameEntity>(UGameplayStatics::CreateSaveGameObject(USaveGameEntity::StaticClass()));
	Communicator::GetInstance().PlayerTransform = InstigatorPawn->GetActorTransform();
	Communicator::GetInstance().PlayerRotation = InstigatorPawn->GetActorRotation();
	Communicator::GetInstance().PlayerHealth = ((AStoneAgeColonyCharacter*)InstigatorPawn)->Health;
	Communicator::GetInstance().PlayerLevel = ((AStoneAgeColonyCharacter*)InstigatorPawn)->Level;
	Communicator::GetInstance().PlayerExperience = ((AStoneAgeColonyCharacter*)InstigatorPawn)->Experience;
	Communicator::GetInstance().PlayerGold = ((AStoneAgeColonyCharacter*)InstigatorPawn)->Gold;
	Communicator::GetInstance().PlayerInventory = ((AStoneAgeColonyCharacter*)InstigatorPawn)->Inventory;

	// Iterate over actors in world to get details of spawned actors.
	// Saves actors to communicator.
	RegisterActors<AEnemyCharacter>();
	RegisterActors<AGatherableTree>();
	RegisterActors<ABuilding>();
	RegisterActors<ASettlement>();
	RegisterActors<ACraftingStation>();
	RegisterActors<AHouse>();

	// Assign variables to save file (from communicator).
	SaveGameEntity->PlayerTransform = Communicator::GetInstance().PlayerTransform;
	SaveGameEntity->PlayerRotation = Communicator::GetInstance().PlayerRotation;
	SaveGameEntity->PlayerHealth = Communicator::GetInstance().PlayerHealth;
	SaveGameEntity->PlayerLevel = Communicator::GetInstance().PlayerLevel;
	SaveGameEntity->PlayerExperience = Communicator::GetInstance().PlayerExperience;
	SaveGameEntity->PlayerGold = Communicator::GetInstance().PlayerGold;
	SaveGameEntity->test = Communicator::GetInstance().test;
	SaveGameEntity->BottomBarItems = Communicator::GetInstance().BottomBarItems;
	SaveGameEntity->SpawnedCharacterDetails = Communicator::GetInstance().SpawnedCharacterDetails;
	SaveGameEntity->SpawnedGatherableTreeDetails = Communicator::GetInstance().SpawnedGatherableTreeDetails;
	SaveGameEntity->SpawnedBuildingDetails = Communicator::GetInstance().SpawnedBuildingDetails;
	SaveGameEntity->SpawnedSettlementDetails = Communicator::GetInstance().SpawnedSettlementDetails;
	SaveGameEntity->SpawnedCraftingStationDetails = Communicator::GetInstance().SpawnedCraftingStationDetails;
	SaveGameEntity->SpawnedHouseDetails = Communicator::GetInstance().SpawnedHouseDetails;
	SaveGameEntity->ElapsedGameMinutes = Communicator::GetInstance().ElapsedGameMinutes;
	SaveGameEntity->PlayerInventory = Communicator::GetInstance().PlayerInventory;

	//UE_LOG(LogTemp, Warning, TEXT("First location in communicator: %f"), GameSaver->SpawnedCharacterDetails[0].CharacterLocation.X);
	// Save save-file to disk.
	UGameplayStatics::SaveGameToSlot(SaveGameEntity, SaveGameEntity->SaveSlotName, SaveGameEntity->UserIndex);
}

template <typename T>
void GameSaver::RegisterActors()
{
	// We will reset spawned character details and update it with current details.
	
	//T::EmptyCommunicatorDetailsArray();
	T::StaticClass()->GetDefaultObject<T>()->EmptyCommunicatorDetailsArray();

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
			// Only do this if Destroy() on this object is not called.
			if (!Itr->IsPendingKill())
			{
				// Register details to communicator.
				Itr->RegisterActorDetailsToSave();
			}
		}
	}
}