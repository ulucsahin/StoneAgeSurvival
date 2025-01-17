// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLoadManager.h"
#include "SaveGameEntity.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Communicator.h"
//#include "EnemyCharacter.h"
#include "SettlementMember.h"
#include "GatherableTree.h"
#include "Building.h"
#include "UIBottomBar.h"
#include "UIPlayerInventory.h"
#include "StoneAgeColonyCharacter.h"
#include "SurvivalGameState.h"
#include "Structure.h"
#include "Settlement.h"
#include "CraftingStation.h"
#include "House.h"
#include "Farm.h"
#include "Plant.h"
#include "Inventory.h"

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

	// LOAD SYSTEM
	USaveGameEntity* SaveGameEntityLoad = Cast<USaveGameEntity>(UGameplayStatics::CreateSaveGameObject(USaveGameEntity::StaticClass()));
	SaveGameEntityLoad = Cast<USaveGameEntity>(UGameplayStatics::LoadGameFromSlot(SaveGameEntityLoad->SaveSlotName, SaveGameEntityLoad->UserIndex));

	if (SaveGameEntityLoad)
	{
		// Destroy existing characters that should be deleted before loading.
		DestroyActors<ASettlementMember>();
		DestroyActors<AGatherableTree>();
		DestroyActors<ABuilding>();
		DestroyActors<ASettlement>();
		DestroyActors<ACraftingStation>();
		DestroyActors<AHouse>();
		DestroyActors<AFarm>();
		DestroyActors<APlant>();

		// Load varibles to communicator (update with loaded variables).
		Communicator::GetInstance().test = SaveGameEntityLoad->test;
		Communicator::GetInstance().SpawnedCharacterDetails = SaveGameEntityLoad->SpawnedCharacterDetails;
		Communicator::GetInstance().SpawnedGatherableTreeDetails = SaveGameEntityLoad->SpawnedGatherableTreeDetails;
		Communicator::GetInstance().SpawnedBuildingDetails = SaveGameEntityLoad->SpawnedBuildingDetails;
		Communicator::GetInstance().SpawnedSettlementDetails = SaveGameEntityLoad->SpawnedSettlementDetails;
		Communicator::GetInstance().SpawnedCraftingStationDetails = SaveGameEntityLoad->SpawnedCraftingStationDetails;
		Communicator::GetInstance().SpawnedHouseDetails = SaveGameEntityLoad->SpawnedHouseDetails;
		Communicator::GetInstance().SpawnedFarmDetails = SaveGameEntityLoad->SpawnedFarmDetails;
		Communicator::GetInstance().PlayerTransform = SaveGameEntityLoad->PlayerTransform;
		Communicator::GetInstance().PlayerRotation = SaveGameEntityLoad->PlayerRotation;
		Communicator::GetInstance().PlayerHealth = SaveGameEntityLoad->PlayerHealth;
		Communicator::GetInstance().PlayerLevel = SaveGameEntityLoad->PlayerLevel;
		Communicator::GetInstance().PlayerExperience = SaveGameEntityLoad->PlayerExperience;
		Communicator::GetInstance().PlayerGold = SaveGameEntityLoad->PlayerGold;
		Communicator::GetInstance().ElapsedGameMinutes = SaveGameEntityLoad->ElapsedGameMinutes;

		// Load player variables.
		((AStoneAgeColonyCharacter*)InstigatorPawn)->SetActorTransform(SaveGameEntityLoad->PlayerTransform);
		((AStoneAgeColonyCharacter*)InstigatorPawn)->SetActorRotation(SaveGameEntityLoad->PlayerRotation);
		((AStoneAgeColonyCharacter*)InstigatorPawn)->Health = Communicator::GetInstance().PlayerHealth;
		((AStoneAgeColonyCharacter*)InstigatorPawn)->Level = Communicator::GetInstance().PlayerLevel;
		((AStoneAgeColonyCharacter*)InstigatorPawn)->Experience = Communicator::GetInstance().PlayerExperience;
		((AStoneAgeColonyCharacter*)InstigatorPawn)->Gold = Communicator::GetInstance().PlayerGold;
		((AStoneAgeColonyCharacter*)InstigatorPawn)->Inventory->Items = SaveGameEntityLoad->PlayerInventory;
		((AStoneAgeColonyCharacter*)InstigatorPawn)->BottomBar->BarItemIDs = SaveGameEntityLoad->BottomBarItems;

		ASurvivalGameState* CurrentGameState = Cast<ASurvivalGameState>(Communicator::GetInstance().World->GetGameState());
		CurrentGameState->ElapsedGameMinutes = Communicator::GetInstance().ElapsedGameMinutes;

		// Update UI Inventory Elements
		RefreshUI(InstigatorPawn);

		// Spawn saved characters.
		SpawnLoadedActors<AGatherableTree>();
		SpawnLoadedActors<ABuilding>();
		SpawnLoadedActors<ACraftingStation>();
		SpawnLoadedActors<AHouse>();
		SpawnLoadedActors<AFarm>();
		SpawnLoadedActors<ASettlement>();
		SpawnLoadedActors<ASettlementMember>();

	}

	UE_LOG(LogTemp, Warning, TEXT("GameLoader: Game loaded :)."));
}


template <typename T>
void AGameLoadManager::SpawnLoadedActors()
{
	/* Spawn previously saved characters from savefile. */
	T::StaticClass()->GetDefaultObject<T>()->SpawnLoadedActors();
	//T::SpawnLoadedActors();
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
	//T::EmptyCommunicatorDetailsArray();
	T::StaticClass()->GetDefaultObject<T>()->EmptyCommunicatorDetailsArray();
}


void AGameLoadManager::RefreshUI(APawn* InstigatorPawn)
{
	auto UIBottomBar = ((AStoneAgeColonyCharacter*)InstigatorPawn)->BottomBar;
	auto UIPlayerInventory = ((AStoneAgeColonyCharacter*)InstigatorPawn)->UIPlayerInventory;

	if (UIBottomBar)
	{
		UIBottomBar->RestoreBottomBarItemsFromSave();
		UIBottomBar->Refresh();
	}

	if (UIPlayerInventory)
	{
		UIPlayerInventory->Refresh();
	}
}