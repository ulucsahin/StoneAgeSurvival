// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingManager.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/TimerManager.h"

// Sets default values for this component's properties
UBuildingManager::UBuildingManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UBuildingManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UBuildingManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBuildingManager::SetWorld(UWorld* World)
{
	this->World = World;
}

FVector UBuildingManager::ToGridLocation(FVector In)
{
	FVector GridLocation = FVector(
		FMath::RoundToInt(In.X / XGridSize) * XGridSize,
		FMath::RoundToInt(In.Y / YGridSize) * YGridSize,
		FMath::RoundToInt(In.Z / ZGridSize) * ZGridSize
	);

	return GridLocation;
}

FVector UBuildingManager::BuildingSnapLocation()
{
	auto PlayerCamera = Player->GetFirstPersonCameraComponent();
	auto PlayerLookAt = Player->GetActorLocation() + (PlayerCamera->GetForwardVector() * 350);
	auto SnapLocation = ToGridLocation(PlayerLookAt);

	return SnapLocation;
}

ABuilding* UBuildingManager::StartBuilding()
{
	/* Spawns a building and returns spawned building */
	
	// Get location to place building
	FVector BuildingLocation = BuildingSnapLocation();
	FRotator BuildingRotation = FRotator(0.f,0.f,0.f); // TODO: Rotation calculations
	CurrentBuilding = NewObject<ABuilding>();
	
	TSubclassOf<ABuilding> TempClass = CurrentBuilding->GetClass();
	if (World)
	{
		// Spawn Object
		CurrentBuilding = World->SpawnActor<ABuilding>(TempClass, BuildingLocation, FRotator::ZeroRotator);
		CurrentBuilding->PreviewMode(true);
		
		FTimerDelegate TimerDel;

		//Binding the function with specific values
		TimerDel.BindUFunction(this, FName("UpdatePreviewTransform"));

		//Calling UpdatePreviewTransform after 1 seconds with looping
		World->GetTimerManager().SetTimer(TimerHandle, TimerDel, 0.15f, true);
		
		//World->GetTimerManager().SetTimer(TimerHandle, this, &UBuildingManager::UpdatePreviewTransform, 1.0f, true);

		return CurrentBuilding;
	}
	else
	{
		return nullptr;
	}
	
}

void UBuildingManager::CompleteBuilding()
{
	World->GetTimerManager().ClearTimer(TimerHandle);
	CurrentBuilding->CompleteBuilding();
	CurrentBuilding = nullptr;
}

void UBuildingManager::CancelBuilding()
{
	World->GetTimerManager().ClearTimer(TimerHandle);

	if (CurrentBuilding)
	{
		CurrentBuilding->Destroy();
		CurrentBuilding = nullptr;
	}	
}


void UBuildingManager::UpdatePreviewTransform()
{
	FVector NewLocation = BuildingSnapLocation();
	if (CurrentBuilding)
	{
		CurrentBuilding->SetActorLocation(NewLocation);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Building is null"));
	}
	
}

void UBuildingManager::StopUpdatingPreviewTransform()
{
	World->GetTimerManager().ClearTimer(TimerHandle);
}