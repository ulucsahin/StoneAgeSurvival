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
	PrimaryComponentTick.bCanEverTick = false;

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
	auto PlayerLookAt = Player->GetActorLocation() + (PlayerCamera->GetForwardVector() * ForwardBuildingOffset);
	auto SnapLocation = ToGridLocation(PlayerLookAt);

	return SnapLocation;
}

FRotator UBuildingManager::BuildingSnapRotation()
{
	/*Rotates(yaw) building that will be placed according to player rotation. */
	auto PlayerCameraYaw = Player->GetFirstPersonCameraComponent()->GetComponentRotation().Yaw;
	auto BuildingYaw = (FMath::RoundToInt(PlayerCameraYaw / RotationSnap) * RotationSnap) + RotationOffset;
	auto BuildingRotation = FRotator::ZeroRotator;
	BuildingRotation.Yaw = BuildingYaw;

	return BuildingRotation;
}

ABuilding* UBuildingManager::StartBuilding()
{
	/* Spawns a building and returns spawned building */
	
	// Get location to place building
	FVector BuildingLocation = BuildingSnapLocation();
	FRotator BuildingRotation = BuildingSnapRotation();
	CurrentBuilding = NewObject<ABuilding>();
	
	TSubclassOf<ABuilding> TempClass = CurrentBuilding->GetClass();
	if (World)
	{
		// Spawn Object
		CurrentBuilding = World->SpawnActor<ABuilding>(TempClass, BuildingLocation, FRotator::ZeroRotator);
		CurrentBuilding->PreviewMode(true);
		World->GetTimerManager().SetTimer(TimerHandle, this, &UBuildingManager::UpdatePreviewTransform, 0.15f, true);
	
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

	if (CurrentBuilding)
	{
		CurrentBuilding->CompleteBuilding();
		CurrentBuilding = nullptr;
	}
	
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
	UE_LOG(LogTemp, Warning, TEXT("Fuck you gc"));

	if (CurrentBuilding)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Building is available"));
		//CurrentBuilding->SetActorLocation(NewLocation);
		FVector NewLocation = BuildingSnapLocation();
		FRotator NewRotation = BuildingSnapRotation();
		CurrentBuilding->SetActorTransform(FTransform(NewRotation, NewLocation));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Building is null"));
		StopUpdatingPreviewTransform();
	}
	
}

void UBuildingManager::StopUpdatingPreviewTransform()
{
	World->GetTimerManager().ClearTimer(TimerHandle);
}

void UBuildingManager::IncreaseForwardBuildingOffset()
{
	UE_LOG(LogTemp, Warning, TEXT("IncreaseForwardBuildingOffset"));
	if (ForwardBuildingOffset < 1200)
	{
		ForwardBuildingOffset += 400;
	}
	
}
void UBuildingManager::DecreaseForwardBuildingOffset()
{
	UE_LOG(LogTemp, Warning, TEXT("DecreaseForwardBuildingOffset"));
	if (ForwardBuildingOffset >= 750)
	{
		ForwardBuildingOffset -= 400;
	}
}

void UBuildingManager::IncreaseRotationOffset()
{
	RotationOffset += 90;
	RotationOffset = RotationOffset % 360;
	UE_LOG(LogTemp, Warning, TEXT("RotationOffset %d"), RotationOffset);
}

void UBuildingManager::DecreaseRotationOffset()
{
	RotationOffset -= 90;
	if (RotationOffset < 0)
	{
		RotationOffset += 360;
	}
	UE_LOG(LogTemp, Warning, TEXT("RotationOffset %d"), RotationOffset);
}
