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
	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
	TSubclassOf<AEnemyCharacter> EnemyCharacterToSpawn;
	int SpawnCount = 0;

protected:
	virtual void OnUsed(APawn* InstigatorPawn) override;

};

