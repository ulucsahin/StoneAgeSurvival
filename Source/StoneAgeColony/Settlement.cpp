// Fill out your copyright notice in the Description page of Project Settings.

#include "Settlement.h"
#include "StoneAgeColonyCharacter.h"
#include "Structure.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "SurvivalWidget.h"
#include "House.h"
#include "CraftingStation.h"
#include "Communicator.h"
#include "ObjectFactory.h"
#include "SettlementMember.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

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
	//FString CompName = "SettlementArea";
	//AreaDisplayer->Rename(*CompName); // CAUSES CRASH AFTER CLOSING AND REOPENING EDITOR
	AreaDisplayer->SetCollisionProfileName("OverlapAll");
	AreaDisplayer->SetupAttachment(SceneComponent);
	AreaDisplayer->SetStaticMesh(AreaDisplayerMesh);
	AreaDisplayer->SetMaterial(0, AreaDisplayerMaterial);
	AreaDisplayer->SetVisibility(false);
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
	PopulationLimit = 3;
	Level = 1;
	Experience = 0;
	BuildingLimit = 10;
	AreaRadius = 1500.f;
	
	AdjustAreaDisplayerSize();
	AdjustAreaDisplayerLocation();

	MenuRef = "/Game/Uluc/Settlement/SettlementBase/MenuAssets/SettlementMenu.SettlementMenu_C";
}

void ASettlement::BeginPlay()
{
	MakeActiveSettlement(); // this will change later
	NotifyAllSettlementMembers();
}

void ASettlement::NotifyAllSettlementMembers()
{
	TArray<AActor*> FoundPeople;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASettlementMember::StaticClass(), FoundPeople);


	for (auto SettlementMember : FoundPeople)
	{
		auto Member = ((ASettlementMember*)SettlementMember);
		if (Member)
		{
			Member->SetupBelongingSettlement();
		}

	}
}

int ASettlement::GetID()
{
	return ID;
}

void ASettlement::OnUsed(APawn* InstigatorPawn)
{
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
	Structure->OwnerSettlement = this;
	Structures.Add(Structure);
	UpdateStats();
	SendNotification();
}

void ASettlement::DeRegisterStructure(AStructure* Structure)
{
	Structure->OwnerSettlement = nullptr;
	Structures.Remove(Structure);
	UpdateStats();
	SendNotification();
}

void ASettlement::RegisterMember(ASettlementMember* Member)
{
	Members.Add(Member);
	UpdateStats();
	SendNotification();
}

void ASettlement::DeRegisterMember(ASettlementMember* Member)
{
	Members.Remove(Member);
	UpdateStats();
	SendNotification();
}

void ASettlement::SendNotification()
{
	/* Notifies settlement members about any change made to structures so that members will know newly added/removed structures */
	for (auto Member : Members)
	{
		Member->GetNotification();
	}

}

void ASettlement::UpdateStats()
{
	PopulationLimit = 3; // Default value is 3 without any houses.
	for (auto Structure : Structures)
	{
		if (Structure->IsA(AHouse::StaticClass()))
		{
			PopulationLimit += ((AHouse*)Structure)->Capacity;
		}
	}

	CurrentPopulation = 1; // including player
	for (auto Member : Members)
	{
		CurrentPopulation++;
	}

}

void ASettlement::MakeActiveSettlement()
{
	IsActiveSettlement = true; // TODO: need a way to determine which settlement is active when game loaded.
	auto Player = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Player->ActiveSettlement = this;

	// TODO: need to set other settlements deactive
}

void ASettlement::ToggleAreaDisplayer()
{
	if (AreaDisplayer->IsVisible())
	{
		AreaDisplayer->SetVisibility(false);
		AreaDisplayOn = false;
	}
	else
	{
		AreaDisplayer->SetVisibility(true);
		AreaDisplayOn = true;
	}
}


// Save-Load methods
void ASettlement::RegisterActorDetailsToSave()
{
	FSettlementDetails SettlementDetails;

	// Assign details to struct.
	SettlementDetails.ID = ID;
	SettlementDetails.Transform = GetActorTransform();
	SettlementDetails.Name = Name;
	SettlementDetails.CurrentPopulation = CurrentPopulation;
	SettlementDetails.Level = Level;
	SettlementDetails.Experience = Experience;
	SettlementDetails.BuildingLimit = BuildingLimit;
	SettlementDetails.AreaRadius = AreaRadius;
	SettlementDetails.AreaDisplayOn = AreaDisplayOn;
	SettlementDetails.IsActiveSettlement = IsActiveSettlement;

	// Save details as struct to communicator. Which will be used during saving.
	Communicator::GetInstance().SpawnedSettlementDetails.Add(SettlementDetails);
}

void ASettlement::EmptyCommunicatorDetailsArray()
{
	Communicator::GetInstance().SpawnedSettlementDetails.Empty();
}

void ASettlement::SpawnLoadedActors()
{
	/* Spawn previously saved characters from savefile. */

	FActorSpawnParameters SpawnParams;

	static AObjectFactory* Factory = NewObject<AObjectFactory>();
	
	// Iterate over array and saved spawn actors.
	for (auto Details : Communicator::GetInstance().SpawnedSettlementDetails)
	{
		auto ObjectToPlace = Factory->CreateObjectBetter(ID);
		auto ClassToSpawn = ObjectToPlace->GetClass();

		FTransform ActorTransform = Details.Transform;
		ASettlement* SpawnedItem = (ASettlement*)Communicator::GetInstance().World->SpawnActor<AUsableActor>(ClassToSpawn, ActorTransform, SpawnParams);
		SpawnedItem->Name = Details.Name;
		SpawnedItem->CurrentPopulation = Details.CurrentPopulation;
		SpawnedItem->Level = Details.Level;
		SpawnedItem->Experience = Details.Experience;
		SpawnedItem->BuildingLimit = Details.BuildingLimit;
		SpawnedItem->AreaRadius = Details.AreaRadius;
		SpawnedItem->AreaDisplayOn = Details.AreaDisplayOn;
		SpawnedItem->IsActiveSettlement = Details.IsActiveSettlement;

		SpawnedItem->AfterLoadSetup();
		
	}
}

void ASettlement::AfterLoadSetup()
{
	AreaDisplayer->SetVisibility(AreaDisplayOn);
}