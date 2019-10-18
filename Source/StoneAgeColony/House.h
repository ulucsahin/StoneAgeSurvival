// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structure.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "House.generated.h"


// Save details
USTRUCT(BlueprintType)
struct FHouseDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ID")
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialID")
	FString SpecialID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capacity")
	int32 Capacity;
};



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

	// Save-Load Methods
	virtual void RegisterActorDetailsToSave() override;
	virtual void EmptyCommunicatorDetailsArray() override;
	virtual void SpawnLoadedActors() override;

	int32 Capacity;
	TSet<FString> Occupants;
protected:
	virtual void OnUsed(APawn* InstigatorPawn) override;
private:
	FName HouseType;
	FHouseData* Data;

	
};
