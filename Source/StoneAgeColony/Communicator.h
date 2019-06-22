// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "GatherableTree.h"
#include "UsableActor.h"


/**
 * 
 */
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
	TMap<int, AUsableActor*> UsableItemIDMap; // how?
	
	// TMap of ID-Amount pairs for items.
	TMap<int, int> PlayerInventory;

	FTransform PlayerTransform;
	FRotator PlayerRotation;
	float PlayerHealth;
	float PlayerStamina;
	int PlayerLevel;
	int PlayerExperience;
	int PlayerGold;

	float ElapsedGameMinutes;

	// Details of objects that should be saved-loaded
	TArray<FEnemyCharacterDetails> SpawnedCharacterDetails;
	TArray<FGatherableTreeDetails> SpawnedGatherableTreeDetails;

	// Blueprints
	TSubclassOf<AEnemyCharacter> EnemyCharacterToSpawn;
	TSubclassOf<AGatherableTree> GatherableTreeToSpawn;
};
