// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StoneAgeColonyCharacter.h"
#include "Building.h"
#include "BuildingManager.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class STONEAGECOLONY_API UBuildingManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildingManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AStoneAgeColonyCharacter* Player;
	//UCameraComponent* PlayerCamera;

	// Grid Variables
	int32 XGridSize = 400;
	int32 YGridSize = 400;
	int32 ZGridSize = 400;
	int32 RotationSnap = 90;
	int32 RotationOffset = 90;
	int32 ForwardBuildingOffset = 350;

	ABuilding* CurrentBuilding;

	// Location methods
	FVector ToGridLocation(FVector);
	FVector BuildingSnapLocation();
	FRotator BuildingSnapRotation();

	// Building process methods
	ABuilding* StartBuilding();
	void CompleteBuilding();
	void CancelBuilding();
	
	FTimerHandle TimerHandle; // timer for updating building preview

	void SetWorld(UWorld* World);
	UWorld* World;

	UFUNCTION() // ufunction needed for timer
	void UpdatePreviewTransform();

	void StopUpdatingPreviewTransform();

	void IncreaseForwardBuildingOffset();
	void DecreaseForwardBuildingOffset();

	TArray<TSubclassOf<ABuilding>> Buildings;
		
};
