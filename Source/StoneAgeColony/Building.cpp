// Fill out your copyright notice in the Description page of Project Settings.

#include "Building.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"

// Sets default values
ABuilding::ABuilding(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BuildingMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	BuildingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = BuildingMesh;
	//UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Geometry/Meshes/TemplateFloor.TemplateFloor"));
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MultistoryDungeons/Blueprints/FIRST-PERSON_V2/FP_Base/SM_Floor_03.SM_Floor_03"));
	if (Mesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Mesh available"));
		BuildingMesh->SetStaticMesh(Mesh);
	}

	// Set Ghost Material and original Material
	GhostMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/Uluc/BuildingSystem/GhostMaterial.GhostMaterial"));
	OriginalMaterial = BuildingMesh->GetMaterial(0); // TODO: this may not get material correctly when another material is assigned
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABuilding::SetBuildingMesh(int type)
{
	
}

void ABuilding::PreviewMode(bool IsPreview)
{
	/* Switch material between ghost and original according to preview mode or not */
	if (IsPreview)
	{
		BuildingMesh->SetMaterial(0, GhostMaterial);
		IsBuilt = false;
	}
	else
	{
		BuildingMesh->SetMaterial(0, OriginalMaterial);
	}
}

void ABuilding::CompleteBuilding()
{
	IsBuilt = true;
	BuildingMesh->SetMaterial(0, OriginalMaterial);
	BuildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

