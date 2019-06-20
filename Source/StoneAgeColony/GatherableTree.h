// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "GatherableTree.generated.h"

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API AGatherableTree : public AUsableActor
{
	GENERATED_BODY()

public:
	AGatherableTree(const FObjectInitializer& ObjectInitializer);
	const static int ID = 100;
	virtual int GetID() override;
	void RegisterActorDetailsToSave();

protected:
	virtual void BeginPlay() override;
	virtual void OnUsed(APawn* InstigatorPawn) override;

private:
	int WoodAmount;
	float LastGatherTime;
};

USTRUCT(BlueprintType)
struct FGatherableTreeDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	FTransform Transform;
};
