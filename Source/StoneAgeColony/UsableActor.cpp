// Fill out your copyright notice in the Description page of Project Settings.

#include "UsableActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "StoneAgeColonyCharacter.h"
#include "Communicator.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

// Sets default values
AUsableActor::AUsableActor(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	//ID = 0;

	MeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = MeshComp;

	// Set InventoryTexture
	static ConstructorHelpers::FObjectFinder<UTexture2D> InventoryTexObj(TEXT("Texture2D'/Game/Uluc/HUD/ItemIcons/danCry.danCry'"));
	InventoryTexture = InventoryTexObj.Object;

	// Set preview materials and original Material
	GhostMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/Uluc/BuildingSystem/GhostMaterial.GhostMaterial"));
	CollisionMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/Uluc/BuildingSystem/ColiisionMaterial.ColiisionMaterial"));
	OriginalMaterial = MeshComp->GetMaterial(0);

	bOverlapping = false;
}

void AUsableActor::PreInitializeComponents() {
	Super::PreInitializeComponents();
}

//int AUsableActor::GetID()
//{
//	UE_LOG(LogTemp, Warning, TEXT("AUsableActor::GetID"));
//	return 69;
//}

// Called when the game starts or when spawned
void AUsableActor::BeginPlay()
{
	Super::BeginPlay();
	//Communicator::GetInstance().UsableItemIDMap.Add(AUsableActor::StaticClass()->GetDefaultObject<AUsableActor>()->ID, AUsableActor::StaticClass());
	UE_LOG(LogTemp, Warning, TEXT("AUsableActor::BeginPlay"));
}

// Called every frame
void AUsableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUsableActor::SetupType(FString Type)
{
	UE_LOG(LogTemp, Warning, TEXT("AUsableActor::SetupType"));
}

void AUsableActor::OnBeginFocus(){
	// Used by custom PostProcess to render outlines
	MeshComp->SetRenderCustomDepth(true);
}

void AUsableActor::OnEndFocus() {
	// Used by custom PostProcess to render outlines
	MeshComp->SetRenderCustomDepth(false);
}


void AUsableActor::OnUsed(APawn* InstigatorPawn) 
{
	GLog->Log("im used");
	//AUsableActor* temp = NewObject<AUsableActor>();
	AStoneAgeColonyCharacter* PlayerCharacter = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		PlayerCharacter->AddToInventory(this->ID, 1);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PLAYER CHARACTER IS NULL IN AUsableActor::OnUsed"));
	}
	
}

void AUsableActor::OnGathered(APawn* InstigatorPawn)
{
	/* Overridden by gatherable objects */

	UE_LOG(LogTemp, Warning, TEXT("Item is not gatherable."));
}

void AUsableActor::PrintName() 
{
	UE_LOG(LogTemp, Warning, TEXT("IM USABLE ACTOR XD"));
}


void AUsableActor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	// CONTINUE FROM HERE
	// CONTINUE FROM HERE
	// CONTINUE FROM HERE
	// CONTINUE FROM HERE
	// CONTINUE FROM HERE
	if (OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("COLLISION"));
		if (OtherActor != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("COLLISION1.5"));

			auto OtherActorClassName = OtherActor->GetClass()->GetFName().ToString();
			UE_LOG(LogTemp, Warning, TEXT("OtherActorClassName: %s"), *OtherActorClassName);

			// also check for mesh if building, this is to prevent collisions with invisible box components
			if (OtherActorClassName == "BP_Building_C")
			{
				UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(OtherComp);
				if (Mesh != nullptr)
				{
					OverlappingActors.Add(OtherActor);
					if (!bOverlapping)
						OnOverlappingBegin();
				}
			}
			else
			{
				OverlappingActors.Add(OtherActor);
				if (!bOverlapping)
					OnOverlappingBegin();
			}


			
		}
		/*UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(OtherComp);
		if (Mesh != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("COLLISION2"));
			auto OtherActorClassName = OtherActor->GetClass()->GetFName();
			auto asd = OtherActorClassName.ToString();
			UE_LOG(LogTemp, Warning, TEXT("OtherActorClassName: %s"), *asd);

			AActor* Actor = Cast<AActor>(OtherActor);
			if (OtherActor != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("COLLISION33131"));
				OverlappingActors.Add(OtherActor);
				if (!bOverlapping)
					OnOverlappingBegin();
			}
		}*/
	}
}

void AUsableActor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this)
	{
		if (OtherActor != nullptr)
		{
			if (OverlappingActors.Contains(OtherActor))
			{
				
				OverlappingActors.Remove(OtherActor);
				if (OverlappingActors.Array().Num() <= 0)
					OnOverlappingEnd();

			}
		}
		/*UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(OtherComp);
		if (Mesh != nullptr)
		{
			AActor* Actor = Cast<AActor>(OtherActor);
			if (OtherActor)
			{
				if (OverlappingActors.Contains(OtherActor))
				{
					OverlappingActors.Remove(OtherActor);
					if (OverlappingActors.Array().Num() <= 0)
						OnOverlappingEnd();

				}
			}
		}*/
	}
}

void AUsableActor::OnOverlappingBegin()
{
	UE_LOG(LogTemp, Warning, TEXT("AUsableActor::OnOverlappingBegin"));
	bOverlapping = true;
	SetMaterialToGhost();
}

void AUsableActor::OnOverlappingEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("AUsableActor::OnOverlappingEnd"));
	bOverlapping = false;

	SetMaterialToOriginal();
}

void AUsableActor::OnPickedUp()
{
	SetMaterialToGhost();
}

void AUsableActor::SetMaterialToGhost()
{
	if (bOverlapping)
	{
		MeshComp->SetMaterial(0, CollisionMaterial);
	}
	else
	{
		MeshComp->SetMaterial(0, GhostMaterial);
	}
	
}

void AUsableActor::SetMaterialToOriginal()
{
	MeshComp->SetMaterial(0, OriginalMaterial);
}