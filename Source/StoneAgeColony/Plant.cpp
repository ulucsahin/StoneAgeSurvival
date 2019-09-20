// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "StoneAgeColonyCharacter.h"
#include "Farm.h"

APlant::APlant(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> PropertiesDataObject(TEXT("DataTable'/Game/Uluc/DataTables/PlantsDataTable.PlantsDataTable'"));
	if (PropertiesDataObject.Succeeded())
	{
		PropertiesDataTable = PropertiesDataObject.Object;
	}

	CurrentStage = 0;
}

void APlant::OnUsed(APawn* InstigatorPawn) 
{
	Gather(InstigatorPawn);
}

void APlant::SetupType(FString Type)
{
	PlantType = FName(*Type);
	const FString ContextString(TEXT("Plant Type Context"));

	Data = PropertiesDataTable->FindRow<FPlantData>(PlantType, ContextString, true);
	ID = Data->ID;
	MenuRef = Data->Menu;
	YieldedItems = Data->YieldedItems;
	YieldAmounts = Data->YieldAmounts;
	GrowingTimes = Data->GrowingTimes;
	MeshAssetPointers = Data->Meshes;
	NumberOfStages = MeshAssetPointers.Num();

	// Load meshes from asset pointers
	UAssetManager* tmp = NewObject<UAssetManager>();
	FStreamableManager& AssetMgr = tmp->GetStreamableManager();
	for (int i = 0; i < MeshAssetPointers.Num(); i++)
	{
		auto Mesh = MeshAssetPointers[i];
		if (Mesh.IsPending())
		{
			const FStringAssetReference& MeshRef = Mesh.ToStringReference();
			MeshAssetPointers[i] = Cast<UStaticMesh>(AssetMgr.SynchronousLoad(MeshRef));
		}
	}

	// Set default mesh to baby plant
	MeshComp->SetStaticMesh(MeshAssetPointers[0].Get());
	DefaultMesh = MeshComp->GetStaticMesh();
}

void APlant::Gather(APawn* InstigatorPawn)
{
	if (CurrentStage == 0) return; // don't allow gathering at baby stage

	auto Player = (AStoneAgeColonyCharacter*)InstigatorPawn;
	for (int32 i = 0; i < YieldedItems.Num(); i++)
	{
		int32 Item = YieldedItems[i];
		int32 ItemTypeCount = YieldedItems.Num();
		int32 Index = ItemTypeCount * (CurrentStage) + i; // calculations needed since we represent a 2D array as 1D (unreal does not support 2D arrays)

		Player->AddToInventory(Item, YieldAmounts[Index]);
	}

	OwnerFarm->RemovePlant(OwnerSocket);
}

void APlant::Grow()
{
	if (CurrentStage < NumberOfStages-1) // -1 since arrays start from 0
	{
		CurrentStage++;

		// Set default mesh to baby plant
		MeshComp->SetStaticMesh(MeshAssetPointers[CurrentStage].Get());
		DefaultMesh = MeshComp->GetStaticMesh();
		ProgressToNextStage = 0.f;
	}

}

void APlant::StartGrowing()
{
	/* This method is called from owner farm when this plant is planted */
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlant::GrowingStep, ProgressUpdateFrequency, true);
}

void APlant::GrowingStep()
{
	ProgressToNextStage += ProgressUpdateFrequency / GrowingTimes[CurrentStage];

	if (CurrentStage == NumberOfStages - 1)
	{
		CompleteGrowing();
	}

	if (ProgressToNextStage >= 1.f)
	{
		if (CurrentStage < NumberOfStages - 1)
		{
			Grow();
		}
	}

}

void APlant::CompleteGrowing()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	ProgressToNextStage = 0.f;
}