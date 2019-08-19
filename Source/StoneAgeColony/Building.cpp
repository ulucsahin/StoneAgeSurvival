// Fill out your copyright notice in the Description page of Project Settings.

#include "Building.h"
#include "Communicator.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
//#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

// Static Variables
int ABuilding::LastMeshType;
//TSubclassOf<ABuilding> ABuilding::BuildingBlueprint;

// Sets default values
ABuilding::ABuilding(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	BuildingMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	BuildingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BuildingMesh->SetupAttachment(SceneComponent);
	RootComponent = SceneComponent;
	
	// Initialize building types (meshes) TODO: This should not be done for each building. Lots of wasted memory and processing power (not really, these are pointers)
	MeshTypes.Add(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Uluc/BuildingSystem/Meshes/SM_Floor_03.SM_Floor_03"))); // floor
	Materials.Add(MeshTypes[0]->GetMaterial(0));
	
	MeshTypes.Add(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Uluc/BuildingSystem/Meshes/SM_FP_Wall_01.SM_FP_Wall_01"))); // wall
	Materials.Add(MeshTypes[1]->GetMaterial(0));

	MeshTypes.Add(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Uluc/BuildingSystem/Meshes/SM_Doorway_04.SM_Doorway_04"))); // wall
	Materials.Add(MeshTypes[2]->GetMaterial(0));

	MeshTypes.Add(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Uluc/BuildingSystem/Meshes/Door_Bronze_01.Door_Bronze_01"))); // door
	Materials.Add(MeshTypes[3]->GetMaterial(0));

	// Assign initial mesh to last used mesh
	if (MeshTypes[LastMeshType])
	{
		// Initialize default mesh as first type
		BuildingMesh->SetStaticMesh(MeshTypes[LastMeshType]);
		CurrentMeshType = LastMeshType;
		
	}
	SetBoxComponent(true);

	// Set preview materials and original Material
	GhostMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/Uluc/BuildingSystem/GhostMaterial.GhostMaterial"));
	CollisionMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/Uluc/BuildingSystem/ColiisionMaterial.ColiisionMaterial"));
	OriginalMaterial = BuildingMesh->GetMaterial(0);


}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();

	// Get Socket name-type pairs which will be used in attaching objects
	for (auto name : BuildingMesh->GetAllSocketNames())
	{
		if (name.ToString().Contains("Floor"))
		{
			Sockets.Add(MakeTuple(name, EBuildTypes::VE_Floor));
		}
		else if (name.ToString().Contains("Wall"))
		{
			Sockets.Add(MakeTuple(name, EBuildTypes::VE_Wall));
		}
		else if (name.ToString().Contains("Door"))
		{
			Sockets.Add(MakeTuple(name, EBuildTypes::VE_Door));
		}
		
	}

}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ABuilding::SetBuildingMesh(int type)
{
	
}

EBuildTypes ABuilding::GetBuildingType()
{
	// Order is important

	// Regular Floor
	// Regular Wall
	// Wall with doorway
	// Door
	TArray<EBuildTypes> Types = { EBuildTypes::VE_Floor, EBuildTypes::VE_Wall, EBuildTypes::VE_Wall, EBuildTypes::VE_Door};
	return Types[CurrentMeshType];
}

void ABuilding::ChangeMesh(int Type=-1)
{
	/*If Type is -1, then ChangeMesh simply chooses next mesh, 
	otherwise, chooses selected mesh at index "Type" */
	if (Type == -1)
	{
		CurrentMeshType = (CurrentMeshType + 1) % MeshTypes.Num();
	}
	else
	{
		CurrentMeshType = Type;
	}
	

	if (MeshTypes[CurrentMeshType])
	{
		BuildingMesh->SetStaticMesh(MeshTypes[CurrentMeshType]);
		OriginalMaterial = Materials[CurrentMeshType];
		SetBoxComponent(false);
		
		//Box->SetupAttachment(SceneComponent);
	}

	LastMeshType = CurrentMeshType;
	FinalMeshType = CurrentMeshType;
	ComputeSocketsArray();
	
}

void ABuilding::SetBoxComponent(bool Init)
{
	
	UE_LOG(LogTemp, Warning, TEXT("ABuilding::SetBoxComponent why is this called twice?"));
	if (Init)
	{
		Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
		Box->SetupAttachment(SceneComponent);
		Box->OnComponentBeginOverlap.AddDynamic(this, &ABuilding::OnOverlapBegin);
		Box->OnComponentEndOverlap.AddDynamic(this, &ABuilding::OnOverlapEnd);
		Box->SetCollisionProfileName("Overlap All");
	}
	

	float Offset = 100.f;

	auto CollisionBounds = BuildingMesh->Bounds.BoxExtent;
	CollisionBounds.X = CollisionBounds.X / 3;
	CollisionBounds.Y = CollisionBounds.Y / 3;
	CollisionBounds.Z = CollisionBounds.Z / 3;

	FVector OffsetVector(0.f, 0.f, CollisionBounds.Z);
	
	auto Rotation = BuildingMesh->GetComponentRotation();
	// Decrease collision bounds otherwise we get unnecessary collisions
	Box->SetBoxExtent(CollisionBounds); //   FVector(45.f, 45.f, 45.f)
	Box->SetRelativeRotation(Rotation);
	//Box->SetWorldRotation(Rotation);
	Box->SetRelativeLocation(FVector::ZeroVector); //OffsetVector
	//Box->AddRelativeLocation(OffsetVector);
	Box->SetVisibility(true);
	Box->bHiddenInGame = false;
	Box->SetHiddenInGame(false);
	
}

void ABuilding::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		//UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(OtherComp);
		UBoxComponent* Mesh = Cast<UBoxComponent>(OtherComp);
		if (Mesh != nullptr)
		{
			ABuilding* Building = Cast<ABuilding>(OtherActor);
			if (Building != nullptr && Building->GetIsBuilt())
			{
				OverlappingBuildings.Add(Building);
				if (!bOverlapping)
					OnOverlappingBegin();
			}
		}
	}
}

void ABuilding::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this)
	{
		//UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(OtherComp);
		UBoxComponent* Mesh = Cast<UBoxComponent>(OtherComp);
		if (Mesh != nullptr)
		{
			ABuilding* Building = Cast<ABuilding>(OtherActor);
			if (Building && Building->GetIsBuilt())
			{
				if (OverlappingBuildings.Contains(Building))
				{
					OverlappingBuildings.Remove(Building);
					if (OverlappingBuildings.Array().Num() <= 0)
						OnOverlappingEnd();

				}
			}
		}
	}
}

void ABuilding::OnOverlappingBegin()
{
	// if built do not change anything
	if (!bIsBuilt)
	{
		bOverlapping = true;
		BuildingMesh->SetMaterial(0, CollisionMaterial);
	}
	
}

void ABuilding::OnOverlappingEnd()
{
	// if built do not change anything
	if (!bIsBuilt)
	{
		bOverlapping = false;
		BuildingMesh->SetMaterial(0, GhostMaterial);
	}
	
}

void ABuilding::PreviewMode(bool IsPreview)
{
	/* Switch material between ghost and original according to preview mode or not */
	if (IsPreview)
	{
		if (bOverlapping)
		{
			BuildingMesh->SetMaterial(0, CollisionMaterial);
		}
		else
		{
			BuildingMesh->SetMaterial(0, GhostMaterial);
		}
		
		bIsBuilt = false;
	}
	else
	{
		BuildingMesh->SetMaterial(0, OriginalMaterial);
	}
}

bool ABuilding::CompleteBuilding()
{
	/* returns true if successfully completes building, eg no collision*/
	if (!bOverlapping)
	{
		bIsBuilt = true;
		BuildingMesh->SetMaterial(0, OriginalMaterial);
		BuildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		return true;
	}
	else
	{
		return false;
	}
}

void ABuilding::ComputeSocketsArray()
{
	Sockets.Empty();
	for (auto name : BuildingMesh->GetAllSocketNames())
	{
		if (name.ToString().Contains("Floor"))
		{
			Sockets.Add(MakeTuple(name, EBuildTypes::VE_Floor));
		}
		else if (name.ToString().Contains("Wall"))
		{
			Sockets.Add(MakeTuple(name, EBuildTypes::VE_Wall));
		}
		else if (name.ToString().Contains("Door"))
		{
			Sockets.Add(MakeTuple(name, EBuildTypes::VE_Door));
		}
	}
}

TArray<FName> ABuilding::GetSocketsWithType(EBuildTypes SocketType)
{
	TArray<FName> Result;
	this->Sockets;

	for (auto Pair : Sockets)
	{
		if (Pair.Value == SocketType)
			Result.Add(Pair.Key);
	}
	
	return Result;
}
// Save-Load Methods
void ABuilding::EmptyCommunicatorDetailsArray()
{
	Communicator::GetInstance().SpawnedBuildingDetails.Empty();
}

void ABuilding::RegisterActorDetailsToSave()
{
	FBuildingDetails BuildingDetails;

	// Assign details to struct.
	BuildingDetails.Transform = GetActorTransform();
	BuildingDetails.MeshType = CurrentMeshType;

	// Save details as struct to communicator. Which will be used during saving.
	Communicator::GetInstance().SpawnedBuildingDetails.Add(BuildingDetails);

	UE_LOG(LogTemp, Warning, TEXT("Building added to communicator."));
}

void ABuilding::OnBeginFocus() {
	// Used by custom PostProcess to render outlines
	BuildingMesh->SetRenderCustomDepth(true);
	UE_LOG(LogTemp, Warning, TEXT("Focused building"));
}

void ABuilding::OnEndFocus() {
	// Used by custom PostProcess to render outlines
	UE_LOG(LogTemp, Warning, TEXT("Stopped focusing building"));
}

void ABuilding::SpawnLoadedActors()
{
	/* Spawn previously saved characters from savefile. */
	FActorSpawnParameters SpawnParams;

	// Get actor details to spawn from communicator.
	auto ActorToSpawn = Communicator::GetInstance().BuildingBlueprint;
	
	// Iterate over array and saved spawn actors.
	for (auto Details : Communicator::GetInstance().SpawnedBuildingDetails)
	{
		FTransform ActorTransform = Details.Transform;
		auto Spawned = Communicator::GetInstance().World->SpawnActor<ABuilding>(ActorToSpawn, ActorTransform, SpawnParams);
		Spawned->ChangeMesh(Details.MeshType);
		Spawned->CompleteBuilding();
	}
}