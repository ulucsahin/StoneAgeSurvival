// Fill out your copyright notice in the Description page of Project Settings.

#include "Communicator.h"
#include <vector>

void Communicator::Reset() {
	// Reset all variables.
	test = 0;
	PlayerTransform = FTransform();
	PlayerRotation = FRotator();
	PlayerHealth = 100.f;
	float PlayerStamina = 100.f;
	int PlayerLevel = 1;
	int PlayerExperience = 0;
	int PlayerGold = 50;
	SpawnedCharacterDetails.Empty();
	ElapsedGameMinutes = 0.f;

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