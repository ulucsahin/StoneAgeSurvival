// Fill out your copyright notice in the Description page of Project Settings.

#include "PeopleSpawner.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Communicator.h"
#include "StoneAgeColonyCharacter.h"
#include "Spawner.h"
 
void APeopleSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void APeopleSpawner::OnUsed(APawn* InstigatorPawn)
{
	// This method is used for spawning actors.

	// If player has enough gold
	if (((AStoneAgeColonyCharacter*)InstigatorPawn)->Gold >= 10)
	{
		UE_LOG(LogTemp, Warning, TEXT("APeopleSpawner::OnUsed hehee"));
		((AStoneAgeColonyCharacter*)InstigatorPawn)->AddToInventory(this->ID);
		((AStoneAgeColonyCharacter*)InstigatorPawn)->PrintInventory();
		((AStoneAgeColonyCharacter*)InstigatorPawn)->Gold -= 10;
		SpawnCharacter(true);
	}
}

void APeopleSpawner::SpawnCharacter(bool bShouldRegister)
{
	//ASpawner* Spawner = NewObject<ASpawner>();
	FActorSpawnParameters SpawnParams;
	FTransform Transform = GetTransform();
	FVector Location = Transform.GetLocation();
	Location.X += 100.f * SpawnCount;
	Location.Y += 100.f;
	Location.Z += 0.f;
	Transform.SetLocation(Location);

	auto ActorToSpawn = Communicator::GetInstance().EnemyCharacterToSpawn;
	AEnemyCharacter* SpawnedActor = GetWorld()->SpawnActor<AEnemyCharacter>(ActorToSpawn, Transform, SpawnParams);

	SpawnCount += 1;
	UE_LOG(LogTemp, Warning, TEXT("AEnemyCharacter spawned. SpawnCount: %d"), SpawnCount);

	// Register spawned actor to Communicator.

	// Registers are now handled by saver.
	/*if (bShouldRegister)
	{	
		if (SpawnedActor) 
		{
			RegisterActorDetailsToSave(SpawnedActor);
			UE_LOG(LogTemp, Warning, TEXT("AEnemyCharacter is registered to Communicator."), SpawnCount);
		}
	}*/
}

void APeopleSpawner::RegisterActorDetailsToSave(AEnemyCharacter* EnemyCharacter) 
{
	EnemyCharacter->RegisterActorDetailsToSave();
}

