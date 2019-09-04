// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "CraftingMaterial.generated.h"


USTRUCT(BlueprintType)
struct FCraftingMaterialData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FCraftingMaterialData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	TMap<int32, int32> CraftRequirements;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 YieldAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	TAssetPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	FText Description;

	
};


UCLASS()
class STONEAGECOLONY_API ACraftingMaterial : public AUsableActor
{
	GENERATED_BODY()

public:
	ACraftingMaterial(const FObjectInitializer& ObjectInitializer);
	virtual void SetupType(FString Name) override;
	int32 ID;
private:
	FName MaterialType;
	FCraftingMaterialData* Data;
};