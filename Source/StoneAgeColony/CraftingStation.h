// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structure.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "CraftingStation.generated.h"

class USurvivalWidget;
class AStoneAgeColonyCharacter;


// Save details
USTRUCT(BlueprintType)
struct FCraftingStationDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ID")
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	FTransform Transform;

};

USTRUCT(BlueprintType)
struct FStructureData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FStructureData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	TArray<int32> CraftableItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	TAssetPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	FString Menu;

};


UCLASS()
class STONEAGECOLONY_API ACraftingStation : public AStructure
{
	GENERATED_BODY()
	
public:
	ACraftingStation(const FObjectInitializer& ObjectInitializer);

	void SetupType(FString);

	int32 GetID() { return ID; };

	virtual void OpenMenu(APawn* InstigatorPawn) override;

	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	TArray<int32> CraftableItems;

	void StartCrafting(float CraftingTime);

	UFUNCTION() // ufunction needed for timer
	void CraftingStep(float CraftingTime, float UpdateFrequency); // update progress bar of CraftingStationMenu

	void StopCrafting();

	bool CraftingRequirementsMet();

	UPROPERTY()
	AUsableActor* CurrentItem;

	// Save-Load Methods
	virtual void RegisterActorDetailsToSave() override;
	virtual void EmptyCommunicatorDetailsArray() override;
	virtual void SpawnLoadedActors() override;


	int32 CurrentItemID;
	bool CurrentlyCrafting;
	int32 CraftAmount;
	float CraftingProgress;
protected:
	virtual void OnUsed(APawn* InstigatorPawn) override;

private:
	AStoneAgeColonyCharacter* Player;
	int32 ID;
	FName CraftingStationType;
	FStructureData* Data;
	bool MenuOpen;
	FTimerHandle TimerHandle;
};
