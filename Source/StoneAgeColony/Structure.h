// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "UsableActor.h"
//#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Structure.generated.h"

class UUserWidget;
class USurvivalWidget;
class ASettlement;


// Save details
USTRUCT(BlueprintType)
struct FStructureDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 ItemID;
};

UCLASS()
class STONEAGECOLONY_API AStructure : public AUsableActor
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnUsed(APawn* InstigatorPawn) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OpenMenu(APawn* InstigatorPawn);

	// Save-Load methods
	virtual void RegisterActorDetailsToSave();
	virtual void EmptyCommunicatorDetailsArray();
	virtual void SpawnLoadedActors();

	ASettlement* OwnerSettlement;
	FName Name;
protected:
	//UUserWidget* Menu;

	UPROPERTY()
	USurvivalWidget* Menu;
	
	FString MenuRef;
private:
	
	FTableRowBase* Data;
	
};
