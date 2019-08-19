// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structure.h"
//#include "Runtime/Engine/Classes/Engine/DataTable.h"
//#include "Runtime/Engine/Classes/Engine/Texture2D.h"
//#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
//#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "CraftingStation.generated.h"

USTRUCT(BlueprintType)
struct FCraftingStationData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FCraftingStationData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 WoodRequirement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 StoneRequirement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	TAssetPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	TAssetPtr<UUserWidget> Menu;
};


UCLASS()
class STONEAGECOLONY_API ACraftingStation : public AStructure
{
	GENERATED_BODY()
	
public:
	void SetupType(FString);

protected:
	virtual void OnUsed(APawn* InstigatorPawn) override;

private:
	int32 WoodRequirement;
	int32 StoneRequirement;
};
