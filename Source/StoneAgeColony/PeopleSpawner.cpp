// Fill out your copyright notice in the Description page of Project Settings.

#include "PeopleSpawner.h"
#include "Runtime/Engine/Classes/Engine/World.h"
//#include 



void APeopleSpawner::OnUsed(APawn* InstigatorPawn) {
	UE_LOG(LogTemp, Warning, TEXT("APeopleSpawner::OnUsed hehee"));
	SpawnCount += 1;
	FActorSpawnParameters SpawnParams;
	FTransform Transform = GetTransform();
	FVector Location = Transform.GetLocation();
	Location.X += 100.f * SpawnCount;
	Location.Y += 100.f;
	Location.Z += 0.f;
	Transform.SetLocation(Location);
	
	//AEnemyCharacter* SpawnedCharacter = (AEnemyCharacter*)GetWorld()->SpawnActor(AEnemyCharacter::StaticClass(), NAME_None, Location);
	//AEnemyCharacter* SpawnedCharacter = GetWorld()->SpawnActor(EnemyCharacterToSpawn, NAME_None, Location);

	AEnemyCharacter* ActorRef = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyCharacterToSpawn, Transform, SpawnParams);
}