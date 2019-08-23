// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupManager.h"
#include "StoneAgeColonyCharacter.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"


// Sets default values
APickupManager::APickupManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ForwardBuildingOffset = 0;

	// Set preview materials and original Material
	GhostMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/Uluc/BuildingSystem/GhostMaterial.GhostMaterial"));
	CollisionMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/Uluc/BuildingSystem/ColiisionMaterial.ColiisionMaterial"));

	// Initialize object snapper
	ObjectSnapper = NewObject<AObjectSnapper>();

	// Initialize Box Component
	//Box = NewObject<UBoxComponent>(CurrentActor);
}

// Called when the game starts or when spawned
void APickupManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickupManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickupManager::SetWorld(UWorld* World) { this->World = World; }

void APickupManager::SetPlayer(AStoneAgeColonyCharacter* Player) { this->Player = Player; }

bool APickupManager::HandlePickup(AUsableActor* Actor)
{
	/* Returns true or false depending on starting or stopping the pickup
	*  true: started pickup or continues being in pickup mode
	*  false: exiting pickup mode or continues being not in pickup mode
	*  true false returns need a rework?
	*/

	// if CurrentActor is not pickupable then exit from this function.
	if (Actor) { if (!Actor->IsPickupable()) { return true; } }

	// If we already have an item and it is not colliding then drop it to where it is. 
	if (CurrentActor)
	{
		if (!CurrentActor->bOverlapping)
		{
			PlaceObject();
			return false; 
		}
		else
		{
			return true; // we want to stay in Pickup mode since we did not place the object
		}
		
		
	}
	// If we do not have a CurrentItem then pick up that item.
	else
	{
		CurrentActor = Actor;
		
		if (Player)
		{
			if (CurrentActor)
			{
				SetupBoxComponent();
				ActorInitialLocation = CurrentActor->GetActorLocation();
				PlayerActorLocationDifference = ActorInitialLocation - Player->GetActorLocation();
				//CurrentActor->SetActorEnableCollision(false);
				//CurrentActor->MeshComp->MoveIgnoreActors.Add(Player);
				CurrentActor->MeshComp->SetCollisionProfileName("OverlapAll");
				//auto Box = (UBoxComponent*)CurrentActor->FindComponentByClass(UBoxComponent::StaticClass());
				
				World->GetTimerManager().SetTimer(TimerHandle, this, &APickupManager::UpdatePreview, 0.015f, true);
				return true;
			}

			return false;
		}

		return false;
	}
	
}

void APickupManager::CalculateLocationDifference()
{

}

void APickupManager::UpdatePreview()
{
	auto PlayerCamera = Player->GetFirstPersonCameraComponent();
	FVector NewLocation = Player->GetActorLocation() + (PlayerCamera->GetForwardVector() * (PlayerActorLocationDifference.Size() + ForwardBuildingOffset));
	CurrentActor->SetActorLocation(NewLocation);
	ObjectSnapper->SnapToGround(CurrentActor, World, NewLocation, 1.f);
	// We already checked CurrentActor before calling this method
	
}

void APickupManager::PreviewMode(bool IsPreview)
{

}

void APickupManager::SetupBoxComponent()
{
	// IMPORTANT: I TURNED THIS OFF SINCE SNAPPING ALREADY PREVENTS COLLISION MOSTLY. WILL FIX LATER.
	// DONT FORGET TO UNCOMMENT TWO LINES IN PLACE OBJECT METHOD IF YOU SET TYPE TO 0

	int type = 5; // dont forget to comment-uncomment in PlaceObject method too
	if (type == 0)
	{
		auto Box = (UBoxComponent*)CurrentActor->FindComponentByClass(UBoxComponent::StaticClass());
		if (!Box)
		{
			Box = NewObject<UBoxComponent>(CurrentActor);
			Box->RegisterComponent();
			Box->SetBoxExtent(CurrentActor->MeshComp->Bounds.BoxExtent);
			Box->SetVisibility(true);
			Box->bHiddenInGame = false;
			Box->SetHiddenInGame(false);

			auto CurrentMesh = (UStaticMeshComponent*)CurrentActor->GetComponentByClass(UStaticMeshComponent::StaticClass());
			//CurrentMesh->OnComponentBeginOverlap.AddDynamic(CurrentActor, &AUsableActor::OnOverlapBegin); // TODO: it works but collision becomes way too sensitive, fix it.
			//CurrentMesh->OnComponentEndOverlap.AddDynamic(CurrentActor, &AUsableActor::OnOverlapEnd);
			Box->AttachTo(CurrentMesh);
			Box->SetRelativeLocation(FVector(0.f, 0.f, CurrentActor->MeshComp->Bounds.BoxExtent.Z));
			Box->OnComponentBeginOverlap.AddDynamic(CurrentActor, &AUsableActor::OnOverlapBegin);
			Box->OnComponentEndOverlap.AddDynamic(CurrentActor, &AUsableActor::OnOverlapEnd);
			Box->SetCollisionProfileName("OverlapAll");

		}
	}
	else if (type == 1)
	{
		auto CurrentMesh = (UStaticMeshComponent*)CurrentActor->GetComponentByClass(UStaticMeshComponent::StaticClass());
		CurrentMesh->OnComponentBeginOverlap.AddDynamic(CurrentActor, &AUsableActor::OnOverlapBegin); // TODO: it works but collision becomes way too sensitive, fix it.
		CurrentMesh->OnComponentEndOverlap.AddDynamic(CurrentActor, &AUsableActor::OnOverlapEnd);
	}
	

	


}

void APickupManager::PlaceObject()
{
	if (!CurrentActor->bOverlapping)
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
		CurrentActor->MeshComp->SetCollisionProfileName("BlockAll"); // Gotta fix this, not every item blocks all.

		// IMPORTANT: IF TYPE IS 0 UNCOMMENT THESE 2 LINES.
		//auto Box = (UBoxComponent*)CurrentActor->FindComponentByClass(UBoxComponent::StaticClass());
		//Box->DestroyComponent();
		
		CurrentActor = nullptr;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot place object"));
	}
	
}

void APickupManager::CancelPlacingObject()
{
	if (CurrentActor)
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
		CurrentActor->SetActorLocation(ActorInitialLocation);
		CurrentActor->MeshComp->SetCollisionProfileName("BlockAll");
		//CurrentActor->SetActorEnableCollision(true);
		CurrentActor = nullptr;
	}
}

void APickupManager::IncreaseForwardBuildingOffset()
{
	UE_LOG(LogTemp, Warning, TEXT("ForwardBuildingOffset: %d"), ForwardBuildingOffset);
	if (ForwardBuildingOffset < 500)
	{
		ForwardBuildingOffset += 25;
	}
	
}

void APickupManager::DecreaseForwardBuildingOffset()
{
	UE_LOG(LogTemp, Warning, TEXT("ForwardBuildingOffset: %d"), ForwardBuildingOffset);
	if (ForwardBuildingOffset > 0)
	{
		ForwardBuildingOffset -= 25;
	}
}


void APickupManager::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != CurrentActor)
	{
		//UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(OtherComp);
		UBoxComponent* Mesh = Cast<UBoxComponent>(OtherComp);
		if (Mesh != nullptr)
		{
			AActor* Actor = Cast<AActor>(OtherActor);
			if (Actor != nullptr)
			{
			}
		}
	}
}

void APickupManager::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("OnOverlapEnd:"));
}