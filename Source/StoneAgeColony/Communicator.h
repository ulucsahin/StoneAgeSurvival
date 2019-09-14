// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "EnemyCharacter.h"
#include "GatherableTree.h"
#include "UsableActor.h"
#include "Building.h"
#include "Settlement.h"
#include "CraftingStation.h"
#include "House.h"

//struct FHumanCharacterDetails;
//struct FGatherableTreeDetails;
//struct FBuildingDetails;
//
//class AEnemyCharacter;
//class AGatherableTree;
//class ABuilding;
//class UDataTable;


class STONEAGECOLONY_API Communicator
{
public:
	static Communicator& GetInstance()
	{
		static Communicator instance; // Guaranteed to be destroyed.
									  // Instantiated on first use.
		return instance;
	}

	void Reset();
	void Increment();
	void SetPlayerLocation(APawn* Player);
	void SetupAssets();

private:
	Communicator() {}

	// C++ 03
	// ========
	// Don't forget to declare these two. You want to make sure they
	// are unacceptable otherwise you may accidentally get copies of
	// your singleton appearing.
	Communicator(Communicator const&); // Don't Implement
	void operator=(Communicator const&); // Don't implement

	// C++ 11
	// =======
	// We can use the better technique of deleting the methods
	// we don't want.

public:
//	Communicator(Communicator const&) = delete;
//	void operator=(Communicator const&) = delete;

	// Game world
	UWorld* World;

	// Variables that will be saved.
	int test;

	// IDs of usable items in game
	//TMap<int, AUsableActor*> UsableItemIDMap; // how?
	//TMap<int, FString> UsableItemIDSetupTypes;
	
	// TMap of ID-Amount pairs for items.
	TMap<int, int> PlayerInventory;

	FTransform PlayerTransform;
	FRotator PlayerRotation;
	float PlayerHealth;
	float PlayerStamina;
	int PlayerLevel;
	int PlayerExperience;
	int PlayerGold;
	TArray<int32> BottomBarItems;

	float ElapsedGameMinutes;

	// Details of objects that should be saved-loaded
	TArray<FHumanCharacterDetails>   SpawnedCharacterDetails;
	TArray<FGatherableTreeDetails>   SpawnedGatherableTreeDetails;
	TArray<FBuildingDetails>         SpawnedBuildingDetails;
	TArray<FSettlementDetails>		 SpawnedSettlementDetails;
	TArray<FCraftingStationDetails>  SpawnedCraftingStationDetails;
	TArray<FHouseDetails>			 SpawnedHouseDetails;

	// Blueprints that are used in save-load system or spawning actors
	TSubclassOf<AEnemyCharacter> EnemyCharacterBlueprint;
	TSubclassOf<AGatherableTree> GatherableTreeBlueprint;
	TSubclassOf<ABuilding>       BuildingBlueprint;
};
