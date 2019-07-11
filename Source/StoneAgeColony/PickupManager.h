// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UsableActor.h"
#include "ObjectSnapper.h"
#include "PickupManager.generated.h"

// To prevent circular dependency
class AStoneAgeColonyCharacter;

UCLASS()
class STONEAGECOLONY_API APickupManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetWorld(UWorld*);
	void SetPlayer(AStoneAgeColonyCharacter*);
	bool HandlePickup(AUsableActor*);
	void CalculateLocationDifference();
	UFUNCTION()
	void UpdatePreview();
	void PreviewMode(bool);
	void SetupBoxComponent();
	void PlaceObject();
	void CancelPlacingObject();
	void IncreaseForwardBuildingOffset();
	void DecreaseForwardBuildingOffset();
private:
	AStoneAgeColonyCharacter* Player;
	UWorld* World;
	UMaterialInterface* OriginalMaterial;
	UMaterial* GhostMaterial;
	UMaterial* CollisionMaterial;
	AUsableActor* CurrentActor;
	FTimerHandle TimerHandle;
	FVector PlayerActorLocationDifference;
	FVector ActorInitialLocation;
	int32 ForwardBuildingOffset;
	AObjectSnapper* ObjectSnapper;
	

public:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	

};
