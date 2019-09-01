// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "GatherableTree.h"
#include "ProceduralEntityPlacer.generated.h"

class AGatherableTree;

UCLASS()
class STONEAGECOLONY_API AProceduralEntityPlacer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralEntityPlacer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	int HowMany;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float AreaRadius;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<UBlueprint*> ObjectsToSpawn;

	TSubclassOf<class AGatherableTree> MyItemBlueprint;

private:
	void AdjustHeight(AActor*);
	void AdjustToSurfaceNormal(AActor*, FHitResult*);
};
