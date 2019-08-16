// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structure.h"
#include "Components/SphereComponent.h"
#include "UsableActor.h"
#include "Settlement.generated.h"

//
class AStoneAgeColonyCharacter;

UCLASS()
class STONEAGECOLONY_API ASettlement : public AUsableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASettlement(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

public:	
	 //Called every frame
	//virtual void Tick(float DeltaTime) override;

	void AddStructure(AStructure* Structure);

	UFUNCTION()
	void OnOverlapBeginSettlement(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEndSettlement(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void AdjustAreaDisplayerSize();
	void AdjustAreaDisplayerLocation();

	// TEST
	AStoneAgeColonyCharacter* Player;

private:
	USceneComponent* SceneComponent;
	USphereComponent* SettlementArea;
	UStaticMeshComponent* AreaDisplayer;
	UMaterial* AreaDisplayerMaterial;
	float OriginalAreaDisplayerSize;

	// Properties
	FName Name;
	int32 PopulationLimit;
	int32 Level;
	int32 Experience;
	int32 BuildingLimit;
	float AreaRadius;
	TArray<AStructure*> Structures;


};
