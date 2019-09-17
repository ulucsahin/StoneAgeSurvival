// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structure.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Farm.generated.h"

class AFarmPlot;

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

	int32 ID;
	int32 PlotCapacity; 

protected:
	virtual void OnUsed(APawn* InstigatorPawn) override;

private:
	//int32 ID;
	FName FarmType;
	FFarmData* Data;
	TArray<AFarmPlot*> FarmPlots;
};
