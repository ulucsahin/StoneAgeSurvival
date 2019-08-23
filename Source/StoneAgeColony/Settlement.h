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
	int ID = 10000;

	void AddStructure(AStructure* Structure);

	//UFUNCTION()
	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	//UFUNCTION()
	virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	void AdjustAreaDisplayerSize();
	void AdjustAreaDisplayerLocation();

	void RegisterStructure(AStructure* Structure);
	void DeRegisterStructure(AStructure* Structure);
	
	virtual int GetID() override;
	// TEST
	AStoneAgeColonyCharacter* Player;

private:
	
	USceneComponent* SceneComponent;
	USphereComponent* SettlementArea;
	UStaticMeshComponent* AreaDisplayer;
	UStaticMeshComponent* SettlementMesh;
	UMaterial* AreaDisplayerMaterial;
	float OriginalAreaDisplayerSize;

	// Properties
	FName Name;
	int32 PopulationLimit;
	int32 Level;
	int32 Experience;
	int32 BuildingLimit;
	float AreaRadius;
	TSet<AStructure*> Structures;


};
