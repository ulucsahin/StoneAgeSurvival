// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingManager.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshSocket.h"

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
	
	// Only execute if we completed building before.
	if (CurrentBuilding == nullptr)
	{
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
			//CurrentBuilding->SetScale(0.66f);
			World->GetTimerManager().SetTimer(TimerHandle, this, &UBuildingManager::UpdatePreview, 0.15f, true);
			
			return CurrentBuilding;
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}
	
	
}

bool UBuildingManager::CompleteBuilding()
{
	if (CurrentBuilding)
	{
		bool Success = CurrentBuilding->CompleteBuilding();
		if (Success)
		{
			World->GetTimerManager().ClearTimer(TimerHandle);
			LastBuildingPlayerLookedAt = CurrentBuilding;
			CurrentBuilding = nullptr;
			return Success;
		}
		return false;
	}
	return false;
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
void UBuildingManager::UpdatePreview()
{
	if (CurrentBuilding)
	{
		auto BuildingPlayerLookingAt = Player->GetActorInView<ABuilding>(InteractRange);
		if (BuildingPlayerLookingAt)
		{
			LastBuildingPlayerLookedAt = BuildingPlayerLookingAt;
		}
		else // if player is not directly looking at a building, check distance to sockets, if too far, detach.
		{
			if (LastBuildingPlayerLookedAt)
			{
				auto Distance_Index = CalculateMinDistanceSocket(LastBuildingPlayerLookedAt, CurrentBuilding->GetBuildingType()); // no deconstruct????
				auto Distance = Distance_Index.Key; // retarded solutions
				auto Index = Distance_Index.Value;

				if (Distance > 750.f)
				{
					if (CurrentBuildingAttached)
					{
						DetachFrom();
						CurrentBuildingAttached = false;
					}
					
				}
				else
				{
					AttachTo(BuildingPlayerLookingAt);
					CurrentBuildingAttached = true;
				}

			}

			if (!CurrentBuildingAttached)
			{
				auto PlayerCamera = Player->GetFirstPersonCameraComponent();
				FVector NewLocation = Player->GetActorLocation() + (PlayerCamera->GetForwardVector() * ForwardBuildingOffset); //BuildingSnapLocation();
				FRotator NewRotation = BuildingSnapRotation();

				CurrentBuilding->SetActorTransform(FTransform(NewRotation, NewLocation));
			}
			

		}

	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Building is null."));
		StopUpdatingPreview();
	}
}

void UBuildingManager::StopUpdatingPreview()
{
	World->GetTimerManager().ClearTimer(TimerHandle);
}

void UBuildingManager::IncreaseForwardBuildingOffset()
{
	if (ForwardBuildingOffset < 1200)
	{
		ForwardBuildingOffset += 400;
	}
	
}
void UBuildingManager::DecreaseForwardBuildingOffset()
{
	if (ForwardBuildingOffset >= 750)
	{
		ForwardBuildingOffset -= 400;
	}
}

void UBuildingManager::IncreaseRotationOffset()
{
	RotationOffset += 90;
	RotationOffset = RotationOffset % 360;
}

void UBuildingManager::DecreaseRotationOffset()
{
	RotationOffset -= 90;
	if (RotationOffset < 0)
	{
		RotationOffset += 360;
	}
}

void UBuildingManager::ChangeBuildingType()
{
	CurrentBuilding->ChangeMesh();
}

FName UBuildingManager::AttachTo(ABuilding* BuildingLookedAt)
{
	// Select socket which player is looking at
	EBuildTypes CurrentBuildingType = CurrentBuilding->GetBuildingType();
	FName SocketNameToAttach = SelectSocketToAttach(CurrentBuildingType);
	
	if (SocketNameToAttach.ToString() == "")
	{
		UE_LOG(LogTemp, Warning, TEXT("OOPS"));
	}

	CurrentBuilding->AttachToComponent(LastBuildingPlayerLookedAt->BuildingMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketNameToAttach);
	CurrentBuilding->PreviewMode(true);

	return SocketNameToAttach;
}

void UBuildingManager::DetachFrom()
{
	CurrentBuilding->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
}

FName UBuildingManager::SelectSocketToAttach(EBuildTypes SocketType)
{
	/* Select closest socket to player's LookAt EndTrace location.
	Selects sockets among sockets of LastBuildingPlayerLookedAt
	*/

	auto Result = CalculateMinDistanceSocket(LastBuildingPlayerLookedAt, SocketType);
	auto ClosestSocketIndex = Result.Value; // value = index

	return ClosestSocketIndex;
}

TTuple<float, FName> UBuildingManager::CalculateMinDistanceSocket(ABuilding* Building, EBuildTypes SocketType)
{
	/* Finds closest socket to player's LookAt EndTrace Location for desired Building.*/

	FVector CamLoc;
	FRotator CamRot;
	Player->Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * InteractRange);

	auto SocketNames = Building->GetSocketsWithType(SocketType); 

	float SmallestDistance = 999999.f;
	FName ClosestSocketName = "";
	for (auto temp : SocketNames)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Socket Name: %s"), *temp.ToString());
		FString SocketName = temp.ToString();
		FVector SocketLocation = Building->BuildingMesh->GetSocketLocation(*SocketName);

		float Distance = FVector::Dist(TraceEnd, SocketLocation);
		if (Distance < SmallestDistance)
		{
			SmallestDistance = Distance;
			ClosestSocketName = temp;
		}

	}
	
	return MakeTuple(SmallestDistance, ClosestSocketName);
}