// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "UsableActor.h"
#include "GatherableObject.h"
#include "GatherableTree.generated.h"

// Object details
USTRUCT(BlueprintType)
struct FGatherableData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FGatherableData() {}

	// Item ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 ID;

	// Gathered Item ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 GatherID;

	/** Icon to use for Achivement */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	TAssetPtr<UTexture2D> Icon;

};


// Save details
USTRUCT(BlueprintType)
struct FGatherableTreeDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	FTransform Transform;

	int WoodAmount;
};


UCLASS()
class STONEAGECOLONY_API AGatherableTree : public AGatherableObject
{
	GENERATED_BODY()

public:
	AGatherableTree(const FObjectInitializer& ObjectInitializer);

	virtual void SetupType(FString) override;

	int ID = 100;
	virtual int GetID() override;

	virtual bool IsPickupable() override;

	// Save-Load Methods
	void RegisterActorDetailsToSave();
	static void EmptyCommunicatorDetailsArray();
	static void SpawnLoadedActors();

protected:
	virtual void BeginPlay() override;
	virtual void OnUsed(APawn* InstigatorPawn) override;
	virtual void OnGathered(APawn* InstigatorPawn) override;

private:
	int WoodAmount;
	float LastGatherTime;
	bool bIsPickupable = false;
	FGatherableData* Data;
};

