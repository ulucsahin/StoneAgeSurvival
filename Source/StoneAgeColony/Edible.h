// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Edible.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FEdibleData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FEdibleData(): Health(0), Stamina(0) {}

	/** XP to get to the given level from the previous level */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 Health;

	/** Extra HitPoints gained at this level */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 Stamina;

	/** Icon to use for Achivement */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	TAssetPtr<UTexture> Icon;

};


UCLASS()
class STONEAGECOLONY_API AEdible : public AUsableActor
{
	GENERATED_BODY()
	
public:
	AEdible(const FObjectInitializer& ObjectInitializer);

	class UDataTable* PropertiesDataTable;

	float Health;

	void SetupEdibleType(FString);
	void Use(APawn*);
	void test();

private:
	FName EdibleType;
	FEdibleData* Data;

};
