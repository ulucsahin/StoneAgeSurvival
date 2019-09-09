// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structure.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
//#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "CraftingStation.generated.h"

class USurvivalWidget;
class AStoneAgeColonyCharacter;

USTRUCT(BlueprintType)
struct FCraftingStationData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FCraftingStationData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	TMap<int32, int32> CraftRequirements;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	TArray<int32> CraftableItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	TAssetPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	TAssetPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	FString Menu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	FText Description;

};


UCLASS()
class STONEAGECOLONY_API ACraftingStation : public AStructure
{
	GENERATED_BODY()
	
public:
	ACraftingStation(const FObjectInitializer& ObjectInitializer);

	void SetupType(FString);

	int32 GetID() { return ID; };

	void OpenMenu(APawn* InstigatorPawn);

	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//TMap<int32, int32> CraftRequirements; //moved to AUsableActor base class
	TArray<int32> CraftableItems;

	UPROPERTY()
	USurvivalWidget* Menu;

	void StartCrafting(float CraftingTime);

	UFUNCTION() // ufunction needed for timer
	void CraftingStep(float CraftingTime, float UpdateFrequency); // update progress bar of CraftingStationMenu

	void StopCrafting();

	bool CraftingRequirementsMet();

	UPROPERTY()
	AUsableActor* CurrentItem;

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
	FCraftingStationData* Data;
	bool MenuOpen;
	FTimerHandle TimerHandle;
};
