// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SurvivalGameInstance.h"
#include "EnemyCharacter.h"
#include "GameSaver.generated.h"



/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API UGameSaver : public USaveGame
{
	GENERATED_BODY()
	
public:
	UGameSaver();

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int test;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FVector PlayerLocation;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int32 TimeOfDay;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<struct FEnemyCharacterDetails> SpawnedCharacterDetails;

	// Will be deleted, saving pointers is useless.
	UPROPERTY(VisibleAnywhere, Category = Basic)
	USurvivalGameInstance* GameInstance;

};

