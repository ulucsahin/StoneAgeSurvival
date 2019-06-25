// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"

UCLASS()
class STONEAGECOLONY_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsBuilt = false;

	UStaticMeshComponent* BuildingMesh;

	// Ghost Material for building preview
	UMaterial* GhostMaterial;

	// Original Material for when preview ends
	UMaterialInterface* OriginalMaterial;

	void SetBuildingMesh(int type);
	void PreviewMode(bool);
	void CompleteBuilding();
};
