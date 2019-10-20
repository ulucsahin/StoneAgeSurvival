// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "StoneAgeColonyCharacter.h"
#include "Farm.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "PlantProgressBar.h"
#include "StoneAgeColonyCharacter.h"
#include "Inventory.h"

APlant::APlant(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> PropertiesDataObject(TEXT("DataTable'/Game/Uluc/DataTables/PlantsDataTable.PlantsDataTable'"));
	if (PropertiesDataObject.Succeeded())
	{
		PropertiesDataTable = PropertiesDataObject.Object;
	}

	// Setup ProgressBarWidget
	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FStringClassReference MyWidgetClassRef("WidgetBlueprint'/Game/Uluc/Plants/PlantProgressBar.PlantProgressBar_C'");
	UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UPlantProgressBar>();
	ProgressBarWidget = CreateWidget<UPlantProgressBar>(PlayerController, MyWidgetClass);
	//ProgressBarWidget->OwnerPlant = this;

	// Initialize ProgressBar component
	ProgressBar = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("TestWidget3D"));
	SetupProgressBar();

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

	Player = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void APlant::SetupProgressBar()
{
	//ProgressBar->RegisterComponent();
	//ProgressBar->SetOnlyOwnerSee(true);
	auto Z = DefaultMesh->GetBoundingBox().GetSize().X;
	ProgressBar->AttachTo(MeshComp);
	ProgressBar->RelativeLocation = FVector(0.f, 0.f, Z + 100.f);
	ProgressBar->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f); // PlayerCamera->GetComponentRotation();
	ProgressBar->SetWidget(ProgressBarWidget);
	ProgressBar->SetWidgetSpace(EWidgetSpace::World);

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

		Player->Inventory->AddItem(Item, YieldAmounts[Index]);
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
	StartUpdatingProgressBar();
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
	ProgressToNextStage = 0.f;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	StopUpdatingProgressBar();
}

void APlant::UpdateProgressBar()
{
	// Set Rotation
	FRotator Rotation;
	Rotation.Yaw = Player->GetFirstPersonCameraComponent()->GetComponentRotation().Yaw + 180.f;
	Rotation.Pitch = 0.f;
	Rotation.Roll = 0.f;
	ProgressBar->SetWorldRotation(Rotation);

	// Set Percentage
	ProgressBarWidget->Percentage = ProgressToNextStage;
}

void APlant::StartUpdatingProgressBar()
{
	ProgressBarWidget->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(TimerHandleProgressBar, this, &APlant::UpdateProgressBar, WidgetProgressUpdateFrequency, true);
}

void APlant::StopUpdatingProgressBar()
{
	ProgressBarWidget->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandleProgressBar);
}

//
// Save-Load Methods
//

FPlantDetails APlant::GetDetails()
{
	/* This method is called by owner farm object. */

	FPlantDetails Details;
	Details.ID = Data->ID;
	Details.GrowProgress = ProgressToNextStage;
	Details.GrowStage = CurrentStage;
	Details.Transform = GetActorTransform();

	return Details;
}

void APlant::ApplyDetails()
{
	MeshComp->SetStaticMesh(MeshAssetPointers[CurrentStage].Get());
	DefaultMesh = MeshComp->GetStaticMesh();
}

void APlant::RegisterActorDetailsToSave()
{

}

void APlant::EmptyCommunicatorDetailsArray()
{

}

void APlant::SpawnLoadedActors()
{

}