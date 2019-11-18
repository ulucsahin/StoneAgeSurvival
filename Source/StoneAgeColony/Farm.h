// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structure.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Plant.h"
#include "CraftingStation.h"
#include "Farm.generated.h"

class AObjectFactory;
class AUsableActor;
class AStoneAgeColonyCharacter;

//class APlant;

// Save details
USTRUCT(BlueprintType)
struct FFarmDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ID")
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	FTransform Transform;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plants")
	TMap<FString, FPlantDetails> PlantDetailsInSockets;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plants")
	TArray<FPlantDetails> PlantDetails;*/
	
};

USTRUCT(BlueprintType)
struct FFarmData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FFarmData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 PlotCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	TAssetPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	FString Menu;

};


UCLASS()
class STONEAGECOLONY_API AFarm : public ACraftingStation
{
	GENERATED_BODY()

public:
	AFarm(const FObjectInitializer& ObjectInitializer);
	void SetupType(FString);
	APlant* Plant(int32 ItemIDToPlant, FName SocketName, bool FromSave);
	void RemovePlant(FString SocketName);
	void RandomizePlantAppearance(APlant* Plant);
	FName SelectSocketToPlant();
	TArray<bool> GetPlantProgress();

	int32 PlotCapacity; 

	//
    // Save-Load Methods below
	virtual void RegisterActorDetailsToSave() override;
	virtual void EmptyCommunicatorDetailsArray() override;
	virtual void SpawnLoadedActors() override;


	TMap<FString, bool> SocketFull; // keeps track of farm plots (planted or empty), true = planted, false = empty, socket name - bool
	TMap<FString, AUsableActor*> PlantsInSockets; // socket name - actor ptr
protected:
	virtual void OnUsed(APawn* InstigatorPawn) override;
	virtual void OpenMenu(APawn* InstigatorPawn) override;

private:
	FString SocketName = "Plot";
	AObjectFactory* Factory;
	FName FarmType;
	FFarmData* Data;
	AStoneAgeColonyCharacter* Player;

	
};
