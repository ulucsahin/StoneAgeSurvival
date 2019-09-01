// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "EnemyCharacter.h"
#include "Spawner.generated.h"

class AEnemyCharacter;

UCLASS()
class STONEAGECOLONY_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
	TSubclassOf<AEnemyCharacter> EnemyCharacterToSpawn;

	int SpawnCount = 0;
	void SpawnCharacter(bool bShouldRegister, FTransform);
	void RegisterActorDetailsToSave(AEnemyCharacter*);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
