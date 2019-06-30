// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Building.generated.h"


enum class EBuildTypes : uint8
{
	VE_Floor  UMETA(DisplayName = "Floor"),
	VE_Wall   UMETA(DisplayName = "Wall")
};

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

	bool bIsBuilt = false;
	bool GetIsBuilt() { return bIsBuilt; }

	bool bOverlapping = false;
	bool IsOverlapping() { return bOverlapping; }

	TSet<ABuilding*> OverlappingBuildings;

	TArray<UStaticMesh*> MeshTypes; // building types
	int CurrentMeshType = 0;
	static int LastMeshType;

	USceneComponent* SceneComponent;
	UStaticMeshComponent* BuildingMesh;
	UBoxComponent* Box;

	// Certain type of building can only be attached to certain types of sockets
	TArray< TTuple<FName, EBuildTypes> > Sockets;
	TArray<FName> GetSocketsWithType(EBuildTypes);

	EBuildTypes GetBuildingType();

	// Ghost Material and Collision Material for building preview
	UMaterial* GhostMaterial;
	UMaterial* CollisionMaterial;

	// Collision Methods
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnOverlappingBegin();

	UFUNCTION()
	void OnOverlappingEnd();

	// Original Material for when preview ends
	UMaterialInterface* OriginalMaterial;

	void SetBuildingMesh(int type);
	void ChangeMesh();
	void PreviewMode(bool);
	bool CompleteBuilding();

	
	void SetScale(float);

	/* Player is looking at */
	virtual void OnBeginFocus();

	/* Player is no longer looking at */
	virtual void OnEndFocus();

private:
	void ComputeSocketsArray();
};
