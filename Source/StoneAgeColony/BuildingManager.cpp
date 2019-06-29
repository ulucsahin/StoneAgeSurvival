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
				auto Distance_Index = CalculateMinDistanceToSockets(LastBuildingPlayerLookedAt); // no deconstruct????
				auto Distance = Distance_Index.Key; // retarded solutions
				auto Index = Distance_Index.Value;

				if (Distance > 750.f)
				{
					if (CurrentBuildingAttached)
					{
						DetachFrom(LastBuildingPlayerLookedAt, 0);
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

int UBuildingManager::AttachTo(ABuilding* BuildingLookedAt)
{
	// Select socket which player is looking at
	int SocketIndexToAttach = SelectSocketToAttach(nullptr);

	auto SocketName = LastBuildingPlayerLookedAt->Sockets[SocketIndexToAttach].Key; // Sockets is Name-type as key-value pairs.

	// We do not need to check SocketTaken array because Attach-Detach functions already do that in Unreal Engine

	//if (Building->SocketTaken[SocketIndexToAttach] == false)
	//{

	auto test = SocketName.ToString();

	UE_LOG(LogTemp, Warning, TEXT("Socket Not Taken."));
	CurrentBuilding->AttachToComponent(LastBuildingPlayerLookedAt->BuildingMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	UE_LOG(LogTemp, Warning, TEXT("SOCKET NAME %s"), *test);
	CurrentBuilding->PreviewMode(true);

	LastBuildingPlayerLookedAt->SocketTaken[SocketIndexToAttach] = true;
	return SocketIndexToAttach;
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Socket Taken."));
	//	return -1;
	//}
	

	
}

void UBuildingManager::DetachFrom(ABuilding* Building, int SocketIndex)
{
	Building->SocketTaken[SocketIndex] = false; // we do not need this array, will be removed.
	CurrentBuilding->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
}

int UBuildingManager::SelectSocketToAttach(ABuilding* BuildingLookedAt)
{
	/* Find which socket is closest to player interaction range (TraceEnd) */

	FVector CamLoc;
	FRotator CamRot;
	Player->Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * InteractRange);
	
	auto SocketNames = LastBuildingPlayerLookedAt->BuildingMesh->GetAllSocketNames(); //const TArray<USkeletalMeshSocket*> AllSockets = SkeletalMesh->GetActiveSocketList();

	int SmallestIndex = -1;
	float SmallestDistance = 999999.f;
	int i = 0;
	for (auto temp : SocketNames)
	{
		FString SocketName = temp.ToString();
		FVector SocketLocation = LastBuildingPlayerLookedAt->BuildingMesh->GetSocketLocation(*SocketName);

		auto Distance = FVector::Dist(TraceEnd, SocketLocation);
		if (Distance < SmallestDistance)
		{
			SmallestDistance = Distance;
			SmallestIndex = i;
		}

		i++;
	}

	return SmallestIndex;
}

TTuple<float, int> UBuildingManager::CalculateMinDistanceToSockets(ABuilding* Building)
{
	FVector CamLoc;
	FRotator CamRot;
	Player->Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * InteractRange);

	auto SocketNames = Building->BuildingMesh->GetAllSocketNames();

	int SmallestIndex = -1;
	float SmallestDistance = 999999.f;
	int i = 0;
	for (auto temp : SocketNames)
	{
		FString SocketName = temp.ToString();
		FVector SocketLocation = Building->BuildingMesh->GetSocketLocation(*SocketName);

		float Distance = FVector::Dist(TraceEnd, SocketLocation);
		if (Distance < SmallestDistance)
		{
			SmallestDistance = Distance;
			SmallestIndex = i;
		}

		i++;
	}
	
	return MakeTuple(SmallestDistance, SmallestIndex);
}