// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner.h"
#include "Runtime/Engine/Classes/Engine/World.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawner::SpawnCharacter(bool bShouldRegister, FTransform LocationToSpawn) {
	SpawnCount += 1;
	FActorSpawnParameters SpawnParams;
	AEnemyCharacter* SpawnedActor = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyCharacterToSpawn, LocationToSpawn, SpawnParams);

	// Do not register if game is still loading. 
	// This bool should be false when we are spawning actors from previously saved game.
	// Should only be true if we are spawning while playing.
	if (SpawnedActor)
	{
		if (bShouldRegister)
			RegisterActorDetailsToSave(SpawnedActor);
	}
}


void ASpawner::RegisterActorDetailsToSave(AEnemyCharacter* EnemyCharacter) {
	if(EnemyCharacter)
		EnemyCharacter->RegisterActorDetailsToSave();
}