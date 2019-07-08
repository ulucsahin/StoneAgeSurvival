// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UsableActor.generated.h"

UCLASS()
class STONEAGECOLONY_API AUsableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	//AUsableActor();
	AUsableActor(const FObjectInitializer& ObjectInitializer);

	const static int ID = 0;

	UTexture2D* InventoryTexture;

	virtual void OnUsed(APawn* InstigatorPawn);

	/* Player is looking at */
	virtual void OnBeginFocus();

	/* Player is no longer looking at */
	virtual void OnEndFocus();
	
	virtual void PrintName();
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PreInitializeComponents() override;

	virtual int GetID() { return ID; };

	virtual UTexture2D* GetInventoryTexture() { return InventoryTexture; };

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

	virtual bool IsPickupable() { return bIsPickupable; }

	// Collision Methods
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void OnOverlappingBegin();
	void OnOverlappingEnd();

	void OnPickedUp();
	void SetMaterialToGhost();
	void SetMaterialToOriginal();

	bool bOverlapping;
private:
	UMaterialInterface* OriginalMaterial;
	UMaterial* GhostMaterial;
	UMaterial* CollisionMaterial;
	TSet<AActor*> OverlappingActors;
	bool bIsPickupable = true;
};
