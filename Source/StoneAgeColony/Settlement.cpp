// Fill out your copyright notice in the Description page of Project Settings.

#include "Settlement.h"
#include "StoneAgeColonyCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ASettlement::ASettlement(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it
	//PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	//
	//// Setup settlement area
	SettlementArea = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SettlementArea->SetSphereRadius(AreaRadius);
	SettlementArea->SetCollisionProfileName("OverlapAll");
	SettlementArea->SetupAttachment(SceneComponent);
	SettlementArea->OnComponentBeginOverlap.AddDynamic(this, &ASettlement::OnOverlapBeginSettlement);
	SettlementArea->OnComponentEndOverlap.AddDynamic(this, &ASettlement::OnOverlapEndSettlement);

	//// Setup settlement area displayer
	auto AreaDisplayerMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	AreaDisplayerMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/Uluc/Settlement/Materials/AreaDisplayer_Mat.AreaDisplayer_Mat"));
	//AreaDisplayer = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	AreaDisplayer = MeshComp;

	AreaDisplayer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AreaDisplayer->SetupAttachment(SceneComponent);
	AreaDisplayer->SetStaticMesh(AreaDisplayerMesh);
	AreaDisplayer->SetMaterial(0, AreaDisplayerMaterial);
	auto AreaDisplayerSize = AreaDisplayer->Bounds.BoxExtent;
	OriginalAreaDisplayerSize = AreaDisplayerSize.X;
	RootComponent = SceneComponent;

	// Initialize properties
	Name = FName(TEXT("Prontera"));
	PopulationLimit = 10;
	Level = 1;
	Experience = 0;
	BuildingLimit = 10;
	AreaRadius = 1500.f;
	
	AdjustAreaDisplayerSize();

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

void ASettlement::OnOverlapBeginSettlement(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("ASettlement::OnOverlapBegin.."));
}


void ASettlement::OnOverlapEndSettlement(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("ASettlement::OnOverlapEnd"));
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