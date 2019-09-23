// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structure.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Farm.generated.h"

class AObjectFactory;
class AUsableActor;
class AStoneAgeColonyCharacter;

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
class STONEAGECOLONY_API AFarm : public AStructure
{
	GENERATED_BODY()

public:

	AFarm(const FObjectInitializer& ObjectInitializer);
	void SetupType(FString);
	FName FindSuitableSocket();
	void Plant(int32 ItemIDToPlant, FName SocketName);
	void RemovePlant(FString SocketName);

	int32 ID;
	int32 PlotCapacity; 

protected:
	virtual void OnUsed(APawn* InstigatorPawn) override;
	virtual void OpenMenu(APawn* InstigatorPawn) override;

private:
	FString SocketName = "Plot";
	AObjectFactory* Factory;
	FName FarmType;
	FFarmData* Data;
	TMap<FString, bool> SocketFull; // keeps track of farm plots (planted or empty), true = planted, false = empty, socket name - bool
	TMap<FString, AUsableActor*> PlantsInSockets; // socket name - actor ptr
	AStoneAgeColonyCharacter* Player;
};
