// Fill out your copyright notice in the Description page of Project Settings.

#include "Communicator.h"
#include "EnemyCharacter.h"
#include "GatherableTree.h"
#include "Building.h"


void Communicator::Reset() {
	// Reset all variables.
	test = 0;
	ElapsedGameMinutes = 0.f;

	// TODO : reset player inventory?
	// Reset Player Details
	PlayerTransform = FTransform();
	PlayerRotation = FRotator();
	PlayerHealth = 100.f;
	float PlayerStamina = 100.f;
	int PlayerLevel = 1;
	int PlayerExperience = 0;
	int PlayerGold = 50;

	// Reset object arrays
	SpawnedCharacterDetails.Empty();
	SpawnedGatherableTreeDetails.Empty();
	SpawnedBuildingDetails.Empty();

	// Log to console.
	UE_LOG(LogTemp, Warning, TEXT("Communicator variables are reset."));
}

void Communicator::Increment() {
	/* 
		This is just a test function for debugging.
	*/

	test += 1;
	UE_LOG(LogTemp, Warning, TEXT("current number is %d"), test);
}

void Communicator::SetPlayerLocation(APawn* Player) {
	// Sets player location in communicator class. Does not actually change players location.
	PlayerTransform = Player->GetActorTransform();
}

FString Communicator::GenerateID()
{
	int32 Len = 6;
	FString Result = "000000";

	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < Len; ++i) {
		Result[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}


	return Result;	
}