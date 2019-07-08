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
			StartUpdatingPreview();
			
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

void UBuildingManager::StartUpdatingPreview()
{
	World->GetTimerManager().SetTimer(TimerHandle, this, &UBuildingManager::UpdatePreview, 0.01f, true);
}

void UBuildingManager::UpdatePreview()
{
	auto UpdatePosition = [=]() {
		if (CurrentBuildingAttached)
		{
			DetachFrom();
			CurrentBuildingAttached = false;
		}

		auto PlayerCamera = Player->GetFirstPersonCameraComponent();
		FVector NewLocation = Player->GetActorLocation() + (PlayerCamera->GetForwardVector() * ForwardBuildingOffset); //BuildingSnapLocation();
		FRotator NewRotation = BuildingSnapRotation();

		CurrentBuilding->SetActorTransform(FTransform(NewRotation, NewLocation));
	};

	// Working version saved in txt.
	if (CurrentBuilding)
	{
		auto Building_Socket = SelectSocketToAttach();
		auto Building = Building_Socket.Key;
		auto SocketName = Building_Socket.Value;

		if(Building)
		{
			auto Distance_Name = CalculateMinDistanceSocket(Building, CurrentBuilding->GetBuildingType()); // no deconstruct????
			auto Distance = Distance_Name.Key; // retarded solutions

			if (Distance > ForwardBuildingOffset)
			{
				UpdatePosition();
				return;
			}
			else
			{
				auto Distance_Name = CalculateMinDistanceSocket(Building, CurrentBuilding->GetBuildingType()); // no deconstruct????
				auto Distance = Distance_Name.Key; // retarded solutions
				auto SocketName = Distance_Name.Value;
				if (SocketName != "")
				{
					if (Distance < ForwardBuildingOffset)
					{
						UE_LOG(LogTemp, Warning, TEXT("Attach, Distance: %f, ForwardBuildingOffset: %d, SphereRadius: %f"), Distance, ForwardBuildingOffset, Player->CollisionSphereRadius);
						AttachTo();
						CurrentBuildingAttached = true;
					}
				}
			}

		}
		else
		{
			UpdatePosition();
			return;
		}

		if(Building)
		{
		    
			//else
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("Socket is null"));
			//}
			
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
	CurrentBuildingAttached = false;
}

void UBuildingManager::IncreaseForwardBuildingOffset()
{
	if (ForwardBuildingOffset < 1200)
	{
		ForwardBuildingOffset += 400;
	}

	UE_LOG(LogTemp, Warning, TEXT("ForwardBuildingOffset: %d"), ForwardBuildingOffset);

}
void UBuildingManager::DecreaseForwardBuildingOffset()
{
	if (ForwardBuildingOffset >= 750)
	{
		ForwardBuildingOffset -= 400;
	}

	UE_LOG(LogTemp, Warning, TEXT("ForwardBuildingOffset: %d"), ForwardBuildingOffset);
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

TTuple<ABuilding*, FName> UBuildingManager::AttachTo()
{
	auto BuildingAndSocketToAttach = SelectSocketToAttach(); // key: ABuilding*, value: socket FName
	FName SocketName = BuildingAndSocketToAttach.Value;
	ABuilding* ChosenBuilding = BuildingAndSocketToAttach.Key;

	CurrentBuilding->AttachToComponent(ChosenBuilding->BuildingMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	CurrentBuilding->PreviewMode(true);

	return BuildingAndSocketToAttach;
}

void UBuildingManager::DetachFrom()
{
	CurrentBuilding->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
}

TTuple<ABuilding*, FName> UBuildingManager::SelectSocketToAttach()
{
	auto SmallestDistance = 99999.f;
	FName ClosestSocketName = "";
	ABuilding* ChosenBuilding;
	if (BuildingsNearPlayer.Num() > 0)
	{
		bool FoundAtLeastOne = false;
		for (auto Building : BuildingsNearPlayer)
		{
			auto Result = CalculateMinDistanceSocket(Building, CurrentBuilding->GetBuildingType());
			if (Result.Key < SmallestDistance)
			{
				SmallestDistance = Result.Key;
				ClosestSocketName = Result.Value;
				ChosenBuilding = Building;
				FoundAtLeastOne = true;
			}
		}
		if (FoundAtLeastOne)
		{
			return MakeTuple(ChosenBuilding, ClosestSocketName);
		}
		else
		{
			return MakeTuple((ABuilding*)nullptr, FName());
		}
		
	}
	else
	{
		return MakeTuple((ABuilding*)nullptr, FName());
	}

}

TTuple<float, FName> UBuildingManager::CalculateMinDistanceSocket(ABuilding* Building, EBuildTypes SocketType)
{
	/* Finds closest socket to player's LookAt EndTrace Location for desired Building.*/
	FVector CamLoc;
	FRotator CamRot;
	Player->Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * ForwardBuildingOffset);
	auto SocketNames = Building->GetSocketsWithType(SocketType); 
	float SmallestDistance = 999999.f;
	FName ClosestSocketName = "";
	if (SocketNames.Num() > 0)
	{
		for (auto temp : SocketNames)
		{
			FString SocketName = temp.ToString();
			FVector SocketLocation = Building->BuildingMesh->GetSocketLocation(*SocketName);

			float Distance = FVector::Dist(TraceEnd, SocketLocation);
			if (Distance < SmallestDistance)
			{
				SmallestDistance = Distance;
				ClosestSocketName = temp;
			}

		}
	}
	return MakeTuple(SmallestDistance, ClosestSocketName);
}