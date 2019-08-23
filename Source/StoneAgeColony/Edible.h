// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "Edible.generated.h"

USTRUCT(BlueprintType)
struct FEdibleData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FEdibleData(): Health(0), Stamina(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	TAssetPtr<UTexture2D> Icon;

};


UCLASS()
class STONEAGECOLONY_API AEdible : public AUsableActor
{
	GENERATED_BODY()
	
public:
	AEdible(const FObjectInitializer& ObjectInitializer);

	class UDataTable* PropertiesDataTable;

	int32 ID;

	float Health;

	virtual void SetupType(FString) override;
	virtual void OnUsed(APawn* InstigatorPawn) override;
	virtual void OnUsedFromInventory(APawn* InstigatorPawn) override;
	void test();
	
	int32 GetID() { return ID; };

private:
	FName EdibleType;
	FEdibleData* Data;

};
