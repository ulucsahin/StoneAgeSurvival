// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Plant.generated.h"

class AFarm;
class UWidgetComponent;
class AStoneAgeColonyCharacter;
class UPlantProgressBar;

// Save details
USTRUCT(BlueprintType)
struct FPlantDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ID")
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GrowProgress")
	int32 GrowStage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GrowProgress")
	float GrowProgress;
};

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
	void SetupProgressBar();
	void Gather(APawn* InstigatorPawn);
	void Grow();

	UFUNCTION() // ufunction needed for timer
	void StartGrowing();
	void GrowingStep();
	void CompleteGrowing();

	AFarm* OwnerFarm;
	FString OwnerSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DWidget")
	UWidgetComponent* ProgressBar;

	UFUNCTION() // ufunction needed for timer
	void UpdateProgressBar();
	void StartUpdatingProgressBar();
	void StopUpdatingProgressBar();

	bool IsGrown();

	//
	// Save-Load functions
	FPlantDetails GetDetails();
	void ApplyDetails();
	virtual void RegisterActorDetailsToSave();
	virtual void EmptyCommunicatorDetailsArray();
	virtual void SpawnLoadedActors();
	

	int32 CurrentStage;
	int32 NumberOfStages;
	float ProgressToNextStage;
protected:
	virtual void OnUsed(APawn* InstigatorPawn) override;
private:
	FPlantData* Data;
	FName PlantType;
	FString MenuRef;
	UPlantProgressBar* ProgressBarWidget;
	TArray<int32> YieldedItems;
	TArray<int32> YieldAmounts;
	TArray<float> GrowingTimes;
	TArray<TAssetPtr<UStaticMesh>> MeshAssetPointers;
	TArray<UStaticMesh*> Meshes;
	const float ProgressUpdateFrequency = 0.1f;
	const float WidgetProgressUpdateFrequency = 0.015f; // 60 fps
	FTimerHandle TimerHandle;
	FTimerHandle TimerHandleProgressBar;
	AStoneAgeColonyCharacter* Player;
};

