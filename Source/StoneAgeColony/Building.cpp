// Fill out your copyright notice in the Description page of Project Settings.

#include "Building.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"

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
	//RootComponent = BuildingMesh;
	
	// Initialize building types (meshes) TODO: This should not be done for each building. Lots of wasted memory and processing power
	MeshTypes.Add(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MultistoryDungeons/Blueprints/FIRST-PERSON_V2/FP_Base/SM_Floor_03.SM_Floor_03")));
	MeshTypes.Add(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MultistoryDungeons/Blueprints/FIRST-PERSON_V2/FP_Base/SM_FP_Wall_01.SM_FP_Wall_01")));
	
	// Temporarily assigned to first one
	if (MeshTypes[0])
	{
		// Initialize default mesh as first type
		BuildingMesh->SetStaticMesh(MeshTypes[0]);	
		BuildingType = GetBuildingType(0);
	}

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	// Decrease collision bounds otherwise we get unnecessary collisions
	auto CollisionBounds = BuildingMesh->Bounds.BoxExtent;
	float Offset = 100.f;
	CollisionBounds.X = CollisionBounds.X - Offset;
	CollisionBounds.Y = CollisionBounds.Y - Offset;
	CollisionBounds.Z = CollisionBounds.Z - Offset;

	Box->SetBoxExtent(CollisionBounds); // FVector(90.f, 90.f, 90.f)
	Box->SetCollisionProfileName("OverlapAll");
	Box->SetupAttachment(SceneComponent);

	Box->OnComponentBeginOverlap.AddDynamic(this, &ABuilding::OnOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &ABuilding::OnOverlapEnd);

	
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
		SocketTaken.Add(false);

		bool bIsFloorSocket = false;
		if (name.ToString().Contains("Floor"))
		{
			Sockets.Add(MakeTuple(name, ESocketTypes::VE_FloorSocket));
		}
		else if (name.ToString().Contains("Wall"))
		{
			Sockets.Add(MakeTuple(name, ESocketTypes::VE_WallSocket));
		}
		
		
		
		//auto temp = asdf.ToString();
		//UE_LOG(LogTemp, Warning, TEXT("Socket Name: %s"), *temp);
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

EBuildingTypes ABuilding::GetBuildingType(int Index)
{
	TArray<EBuildingTypes> Types = { EBuildingTypes::VE_Floor, EBuildingTypes::VE_Wall };
	return Types[Index];
}

void ABuilding::ChangeMesh()
{
	// This method is not implemented
	UE_LOG(LogTemp, Warning, TEXT("MeshTypes.Num: %d"), MeshTypes.Num());
	if (MeshTypes[1])
	{
		BuildingMesh->SetStaticMesh(MeshTypes[1]);
		Box->SetBoxExtent(BuildingMesh->Bounds.BoxExtent);
		Box->SetCollisionProfileName("OverlapAll");
		//Box->SetupAttachment(SceneComponent);
	}

}

void ABuilding::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnOverlapBegin"));

	if (OtherActor != this)
	{
		UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(OtherComp);
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
	//UE_LOG(LogTemp, Warning, TEXT("OnOverlapEnd"));
	if (OtherActor != this)
	{
		UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(OtherComp);
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

void ABuilding::SetScale(float Scale)
{
	BuildingMesh->SetWorldScale3D(FVector(Scale, Scale, Scale));
	BuildingMesh->SetRelativeScale3D(FVector(Scale, Scale, Scale));

	Box->SetBoxExtent(BuildingMesh->Bounds.BoxExtent);
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