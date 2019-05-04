// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "EnemyCharacter.h"
#include "PeopleSpawner.generated.h"

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API APeopleSpawner : public AUsableActor
{
	GENERATED_BODY()


public:
	//APeopleSpawner();

	//UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
	//TSubclassOf<AEnemyCharacter> EnemyCharacterToSpawn;

	int SpawnCount = 0;
	void SpawnCharacter(bool bShouldRegister);
	void RegisterActorDetailsToSave(AEnemyCharacter*);

protected:
	virtual void OnUsed(APawn* InstigatorPawn) override;
	virtual void BeginPlay() override;

};

