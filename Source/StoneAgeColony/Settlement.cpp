// Fill out your copyright notice in the Description page of Project Settings.

#include "Settlement.h"
#include "StoneAgeColonyCharacter.h"
#include "Structure.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "SurvivalWidget.h"
//#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ASettlement::ASettlement(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	
	// Setup settlement area
	//SettlementArea = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	//SettlementArea->SetSphereRadius(AreaRadius);
	//SettlementArea->SetCollisionProfileName("NoCollision"); // OverlapAll
	//SettlementArea->SetupAttachment(SceneComponent);
	//SettlementArea->OnComponentBeginOverlap.AddDynamic(this, &ASettlement::OnOverlapBegin);
	//SettlementArea->OnComponentEndOverlap.AddDynamic(this, &ASettlement::OnOverlapEnd);

	// TODO: Area Displayer will be added after settlement is setup. Otherwise creates problems while placing settlement.
	// Setup settlement area displayer
	auto AreaDisplayerMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	AreaDisplayerMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/Uluc/Settlement/Materials/AreaDisplayer_Mat.AreaDisplayer_Mat"));

	//AreaDisplayer = MeshComp; 
	AreaDisplayer = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("SettlementArea")); // MeshComp from UsableActor parent class

	AreaDisplayer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FString CompName = "SettlementArea";
	//AreaDisplayer->Rename(*CompName); // CAUSES CRASH AFTER CLOSING AND REOPENING EDITOR
	AreaDisplayer->SetCollisionProfileName("OverlapAll");
	AreaDisplayer->SetupAttachment(SceneComponent);
	AreaDisplayer->SetStaticMesh(AreaDisplayerMesh);
	AreaDisplayer->SetMaterial(0, AreaDisplayerMaterial);
	AreaDisplayer->SetVisibility(true);
	AreaDisplayer->OnComponentBeginOverlap.AddDynamic(this, &ASettlement::OnOverlapBegin);
	AreaDisplayer->OnComponentEndOverlap.AddDynamic(this, &ASettlement::OnOverlapEnd);
	auto AreaDisplayerSize = AreaDisplayer->Bounds.BoxExtent;
	OriginalAreaDisplayerSize = AreaDisplayerSize.X;
	RootComponent = SceneComponent;


	// Setup main mesh
	MeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("SettlementMesh"));
	DefaultMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Uluc/Settlement/SettlementBase/SettlementBase.SettlementBase")); // temporary mesh
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCollisionProfileName("BlockAll");
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetStaticMesh(DefaultMesh);
	
	// Initialize properties
	Name = "Prontera";
	PopulationLimit = 10;
	Level = 1;
	Experience = 0;
	BuildingLimit = 10;
	AreaRadius = 1500.f;
	
	AdjustAreaDisplayerSize();
	AdjustAreaDisplayerLocation();

	MenuRef = "/Game/Uluc/Settlement/SettlementBase/MenuAssets/SettlementMenu.SettlementMenu_C";
}

int ASettlement::GetID()
{
	return ID;
}

void ASettlement::OnUsed(APawn* InstigatorPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("im a setlmennto"));
	OpenMenu(InstigatorPawn);
}

void ASettlement::OpenMenu(APawn* InstigatorPawn)
{
	/* Prevents opening multiple of same menus */

	// Data->Menu is empty if this station has no menu.
	if (MenuRef == "")
	{
		return;
	}

	// Checks if menu is already open or not.
	if (!Menu)
	{
		Menu = ((AStoneAgeColonyCharacter*)InstigatorPawn)->OpenMenu(MenuRef, this, this);
	}
	else
	{
		if (!Menu->IsActive)
		{
			Menu = ((AStoneAgeColonyCharacter*)InstigatorPawn)->OpenMenu(MenuRef, this, this);
		}
	}
}

void ASettlement::AddStructure(AStructure* Structure)
{
	Structures.Add(Structure);
}

void ASettlement::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	auto OtherActorClassName = OtherActor->GetClass()->GetFName().ToString();
	auto OtherActorCompName = OtherComp->GetFName().ToString();
	if (OtherActorCompName == "BoxComponent_0") return; // BoxComponent_0 is created by PickupManager when carrying items.

	//UE_LOG(LogTemp, Warning, TEXT("ASettlement::OnOverlapBegin --->  OtherActorClassName: %s"), *OtherActorClassName);
	//UE_LOG(LogTemp, Warning, TEXT("ASettlement::OnOverlapBegin --->  OtherActorCompName: %s"), *OtherActorCompName);
	if (OtherActor != this)
	{
		if (OtherActor != nullptr)
		{

			if (OtherActor->IsA(AStructure::StaticClass()))
			{
				RegisterStructure((AStructure*)OtherActor);
			}

		}

	}

}


void ASettlement::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Super::OnOverlapEnd(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
	auto OtherActorClassName = OtherActor->GetClass()->GetFName().ToString();

	if (OtherActor != this)
	{
		if (OtherActor != nullptr)
		{
			if (Structures.Contains((AStructure*)OtherActor))
			{
				DeRegisterStructure((AStructure*)OtherActor);

			}
		}

	}

}

void ASettlement::AdjustAreaDisplayerSize()
{
	/* Adjusts scale of AreaDisplayer so that it matches AreaRadius of settlement. */
	
	float Ratio = AreaRadius / OriginalAreaDisplayerSize;

	AreaDisplayer->SetWorldScale3D(FVector(Ratio, Ratio, Ratio));
	AreaDisplayer->SetRelativeScale3D(FVector(Ratio, Ratio, Ratio));
}

void ASettlement::AdjustAreaDisplayerLocation()
{
	AreaDisplayer->SetRelativeLocation(FVector(0.f, 0.f, -AreaRadius));
	//AreaDisplayer->SetRelativeTransform(FTransform());
}

void ASettlement::RegisterStructure(AStructure* Structure)
{
	Structures.Add(Structure);
	UE_LOG(LogTemp, Warning, TEXT("ASettlement::RegisterStructure Structure Amount: %d"), Structures.Num());
}

void ASettlement::DeRegisterStructure(AStructure* Structure)
{
	Structures.Remove(Structure);
	UE_LOG(LogTemp, Warning, TEXT("ASettlement::DeregisterStructure Structure Amount: %d"), Structures.Num());
}