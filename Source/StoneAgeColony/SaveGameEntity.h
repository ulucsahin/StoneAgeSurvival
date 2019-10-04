// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SurvivalGameInstance.h"
//#include "EnemyCharacter.h"
#include "SettlementMember.h"
#include "GatherableTree.h"
#include "Building.h"
#include "Settlement.h"
#include "CraftingStation.h"
#include "House.h"
#include "Farm.h"
#include "SaveGameEntity.generated.h"

//class AEnemyCharacter;
//class AGatherableTree;
//class ABuilding;
//class USurvivalGameInstance;
//struct FHumanCharacterDetails;
//struct FGatherableTreeDetails;
//struct FBuildingDetails;

UCLASS()
class STONEAGECOLONY_API USaveGameEntity : public USaveGame
{
	GENERATED_BODY()
	
public:
	USaveGameEntity();

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int test;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FTransform PlayerTransform;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FRotator PlayerRotation;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	float PlayerHealth;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	float PlayerStamina;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int PlayerLevel;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int PlayerExperience;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int PlayerGold;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int32 TimeOfDay;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<struct FHumanCharacterDetails> SpawnedCharacterDetails;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<struct FGatherableTreeDetails> SpawnedGatherableTreeDetails;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FBuildingDetails> SpawnedBuildingDetails;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FSettlementDetails> SpawnedSettlementDetails;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FCraftingStationDetails> SpawnedCraftingStationDetails;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FHouseDetails> SpawnedHouseDetails;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FFarmDetails> SpawnedFarmDetails;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	float ElapsedGameMinutes;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TMap<int, int> PlayerInventory;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<int32> BottomBarItems;

	// Will be deleted, saving pointers is useless.
	UPROPERTY(VisibleAnywhere, Category = Basic)
	USurvivalGameInstance* GameInstance;
};
