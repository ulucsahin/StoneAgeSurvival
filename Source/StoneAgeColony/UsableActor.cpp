// Fill out your copyright notice in the Description page of Project Settings.

#include "UsableActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "StoneAgeColonyCharacter.h"
#include "Communicator.h"
#include "ObjectFactory.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/Material.h"

// Sets default values
AUsableActor::AUsableActor(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	MeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));

	// Set default mesh, also used as default world model for items dropped through inventory
	DefaultMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/SoulCave/Environment/Meshes/Bricks/SM_Cave_Brick_01e.SM_Cave_Brick_01e"));
	//MeshComp->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/SoulCave/Environment/Meshes/Building_Slum/SM_Slums_Trashbag.SM_Slums_Trashbag")));
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
	/* Will be implemented by required child classes. */
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
	
	AStoneAgeColonyCharacter* PlayerCharacter = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(Communicator::GetInstance().World, 0);
	if (PlayerCharacter)
	{
		PlayerCharacter->AddToInventory(this->ID, 1);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PLAYER CHARACTER IS NULL IN AUsableActor::OnUsed"));
	}
	
}

void AUsableActor::OnUsedFromInventory(APawn* InstigatorPawn)
{
	GLog->Log("AUsableActor::OnUsedFromInventory");
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
	// what was this ^ ????
	if (OtherActor != this)
	{
		if (OtherActor != nullptr)
		{			
			// If colliding component is SettlementArea, ignore it, return.
			auto OtherActorCompName = OtherComp->GetFName().ToString();
			if (OtherActorCompName == "SettlementArea") return;
			
			// also check for mesh if building, this is to prevent collisions with invisible box components
			auto OtherActorClassName = OtherActor->GetClass()->GetFName().ToString();
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
	}
}

void AUsableActor::OnOverlappingBegin()
{
	bOverlapping = true;
	SetMaterialToGhost();
}

void AUsableActor::OnOverlappingEnd()
{
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

void AUsableActor::SetMeshToDefault()
{
	/* Sets mesh of this object to default world model mesh, which is a bag.
	* Used when dropping items from inventory (If dropped item has no unique mesh)
	*/

	MeshComp->SetStaticMesh(DefaultMesh);

}