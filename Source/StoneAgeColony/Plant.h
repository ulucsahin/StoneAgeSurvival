// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Plant.generated.h"

class AFarm;

USTRUCT(BlueprintType)
struct FPlantData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FPlantData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
	TArray<int32> YieldedItems;

	// different amounts while growing, this is a 2D array represented as 1D array. 
	//For example, if we have 2 different yielded items, 3 stages, this array holds: item1-stage1, item2-stage1, item2-stage1, item2-stage2...
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
	TArray<int32> YieldAmounts; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
	TArray<float> GrowingTimes; // different amounts while growing

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
	TArray<TAssetPtr<UStaticMesh>> Meshes; // different meshes while growing

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
	FString Menu;

};

UCLASS()
class STONEAGECOLONY_API APlant : public AUsableActor
{
	GENERATED_BODY()
	
public:
	APlant(const FObjectInitializer& ObjectInitializer);
	void SetupType(FString);
	void Gather(APawn* InstigatorPawn);
	void Grow();

	UFUNCTION() // ufunction needed for timer
	void StartGrowing();
	void GrowingStep();
	void CompleteGrowing();

	AFarm* OwnerFarm;
	FString OwnerSocket;
protected:
	virtual void OnUsed(APawn* InstigatorPawn) override;
	
private:
	FPlantData* Data;
	FName PlantType;
	int32 CurrentStage;
	int32 NumberOfStages;
	FString MenuRef;
	TArray<int32> YieldedItems;
	TArray<int32> YieldAmounts;
	TArray<float> GrowingTimes;
	TArray<TAssetPtr<UStaticMesh>> MeshAssetPointers;
	TArray<UStaticMesh*> Meshes;
	float ProgressToNextStage;
	float ProgressUpdateFrequency = 0.1f;
	
	FTimerHandle TimerHandle;
};
