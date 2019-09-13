// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structure.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "House.generated.h"

USTRUCT(BlueprintType)
struct FHouseData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FHouseData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 Capacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	TAssetPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	FString Menu;

};


UCLASS()
class STONEAGECOLONY_API AHouse : public AStructure
{
	GENERATED_BODY()
	
public:
	AHouse(const FObjectInitializer& ObjectInitializer);
	void SetupType(FString);

	void OpenMenu(APawn* InstigatorPawn);

	int32 Capacity;
protected:
	virtual void OnUsed(APawn* InstigatorPawn) override;
	
private:
	int32 ID;
	FName HouseType;
	FHouseData* Data;
	
};
