// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structure.h"
#include "Settlement.generated.h"

class AStoneAgeColonyCharacter;
class AStructure;
class AActor;

// Save details
USTRUCT(BlueprintType)
struct FSettlementDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ID")
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	int32 PopulationLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	int32 CurrentPopulation = 1; // TODO: will be changed when we can hire people to our settlement.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	int32 Experience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	int32 BuildingLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float AreaRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	bool AreaDisplayOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	bool IsActiveSettlement;

};


UCLASS()
class STONEAGECOLONY_API ASettlement : public AStructure
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASettlement(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	// virtual void BeginPlay() override;

public:	
	 //Called every frame
	//virtual void Tick(float DeltaTime) override;
	int ID = 10000;

	void AddStructure(AStructure* Structure);

	virtual void BeginPlay() override;

	//UFUNCTION()
	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	//UFUNCTION()
	virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	void AdjustAreaDisplayerSize();
	void AdjustAreaDisplayerLocation();

	void RegisterStructure(AStructure* Structure);
	void DeRegisterStructure(AStructure* Structure);
	
	virtual int GetID() override;

	void UpdateStats();
	
	// Public Properties
	AStoneAgeColonyCharacter* Player;
	FString Name;
	int32 PopulationLimit;
	int32 CurrentPopulation = 1; // TODO: will be changed when we can hire people to our settlement.
	int32 Level;
	int32 Experience;
	int32 BuildingLimit;
	float AreaRadius;
	bool IsActiveSettlement;
	bool AreaDisplayOn;
	TSet<AStructure*> Structures;

	void OpenMenu(APawn* InstigatorPawn);
	void MakeActiveSettlement();
	void ToggleAreaDisplayer();

	// Save-Load Methods
	virtual void RegisterActorDetailsToSave() override;
	virtual void EmptyCommunicatorDetailsArray() override;
	virtual void SpawnLoadedActors() override;
	void AfterLoadSetup();

protected:
	virtual void OnUsed(APawn* InstigatorPawn) override;
private:
	
	USceneComponent* SceneComponent;
	//USphereComponent* SettlementArea;
	UStaticMeshComponent* AreaDisplayer;
	//UStaticMeshComponent* SettlementMesh;
	UMaterial* AreaDisplayerMaterial;
	float OriginalAreaDisplayerSize;
};
