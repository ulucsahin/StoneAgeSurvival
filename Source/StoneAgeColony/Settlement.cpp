// Fill out your copyright notice in the Description page of Project Settings.

#include "Settlement.h"
#include "StoneAgeColonyCharacter.h"
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


	MeshComp;
	// Setup main mesh
	MeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("SettlementMesh"));
	DefaultMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Props/SM_TableRound.SM_TableRound")); // temporary mesh
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCollisionProfileName("BlockAll");
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetStaticMesh(DefaultMesh);
	
	// Initialize properties
	Name = FName(TEXT("Prontera"));
	PopulationLimit = 10;
	Level = 1;
	Experience = 0;
	BuildingLimit = 10;
	AreaRadius = 1500.f;
	
	AdjustAreaDisplayerSize();
	AdjustAreaDisplayerLocation();
}

int ASettlement::GetID()
{
	return ID;
}

//// Called when the game starts or when spawned
//void ASettlement::BeginPlay()
//{
//	Super::BeginPlay();
//
//	//// TEST
//	//AStoneAgeColonyCharacter* Player = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
//	//if (Player)
//	//{
//	//	Player->Settlement = this;
//	//	UE_LOG(LogTemp, Warning, TEXT("Player available."));
//	//}
//}
//
//// Called every frame
//void ASettlement::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

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
				UE_LOG(LogTemp, Warning, TEXT("This is a structure"));
				RegisterStructure((AStructure*)OtherActor);
			}

			
			
			//// also check for mesh if building, this is to prevent collisions with invisible box components
			//if (OtherActorClassName == "BP_Building_C")
			//{
			//	UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(OtherComp);
			//	if (Mesh != nullptr)
			//	{
			//		OverlappingActors.Add(OtherActor);
			//		if (!bOverlapping)
			//			OnOverlappingBegin();
			//	}
			//}
			//else
			//{
			//	OverlappingActors.Add(OtherActor);
			//	if (!bOverlapping)
			//		OnOverlappingBegin();
			//}

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

	UE_LOG(LogTemp, Warning, TEXT("ASettlement::AdjustAreaDisplayerSize ratio: %f"), Ratio);

	AreaDisplayer->SetWorldScale3D(FVector(Ratio, Ratio, Ratio));
	AreaDisplayer->SetRelativeScale3D(FVector(Ratio, Ratio, Ratio));

	auto asd = AreaDisplayer->Bounds.BoxExtent;
	UE_LOG(LogTemp, Warning, TEXT("SIZE: %f %f %f"), asd.X, asd.Y, asd.Z);
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