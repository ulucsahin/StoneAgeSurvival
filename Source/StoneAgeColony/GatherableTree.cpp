// Fill out your copyright notice in the Description page of Project Settings.

#include "GatherableTree.h"
//#include "Runtime/Engine/Classes/Engine/World.h"
#include "Communicator.h"
#include "StoneAgeColonyCharacter.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "ObjectFactory.h"

// Static Variables
//TSubclassOf<AGatherableTree> AGatherableTree::GatherableTreeBlueprint;

AGatherableTree::AGatherableTree(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> PropertiesDataObject(TEXT("DataTable'/Game/Uluc/DataTables/GatherablesDataTable.GatherablesDataTable'"));
	if (PropertiesDataObject.Succeeded())
	{
		PropertiesDataTable = PropertiesDataObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> InventoryTexObj(TEXT("Texture2D'/Game/Uluc/HUD/ItemIcons/TreeIcon.TreeIcon'"));
	InventoryTexture = InventoryTexObj.Object;

	ID = 100;
}

void AGatherableTree::BeginPlay()
{
	WoodAmount = 100;
	LastGatherTime = -100.f;

}

void AGatherableTree::SetupType(FString Type)
{
	auto Type_ = FName(*Type);

	const FString ContextString(TEXT("Gatherable Type Context"));
	Data = PropertiesDataTable->FindRow<FGatherableData>(Type_, ContextString, true);
	ID = Data->ID;
	GatherID = Data->GatherID;

	// Required for loading icon from TAssetPtr with Get()
	if (Data->Mesh.IsPending())
	{
		UAssetManager* tmp = NewObject<UAssetManager>();
		FStreamableManager& AssetMgr = tmp->GetStreamableManager(); //UAssetManager::GetStreamableManager();
		const FStringAssetReference& MeshRef = Data->Mesh.ToStringReference();
		Data->Mesh = Cast<UStaticMesh>(AssetMgr.SynchronousLoad(MeshRef));
	}

	DefaultMesh = Data->Mesh.Get();
}

void AGatherableTree::OnUsed(APawn* InstigatorPawn)
{
	//Super::OnUsed(InstigatorPawn);
}

void AGatherableTree::OnGathered(APawn* InstigatorPawn)
{
	float GatherTime = Communicator::GetInstance().World->GetTimeSeconds();

	// Put harvesting on a cooldown
	// Can this cause int overflow after certain time?
	// dont be dumb use timer handle
	if (GatherTime - LastGatherTime > 2.f)
	{
		WoodAmount -= 10;
		UE_LOG(LogTemp, Warning, TEXT("Tree Harvested at %f , remaining wood: %d"), GatherTime, WoodAmount);
		LastGatherTime = GatherTime;
		int AmountToAdd = 10;

		UE_LOG(LogTemp, Warning, TEXT("AGatherableTree::OnGathered GatherID: %d"), GatherID);
		((AStoneAgeColonyCharacter*)InstigatorPawn)->AddToInventory(GatherID, AmountToAdd);
	}
}

int AGatherableTree::GetID() { return ID; }

bool AGatherableTree::IsPickupable() { return bIsPickupable; }

void AGatherableTree::RegisterActorDetailsToSave() {
	FGatherableTreeDetails TreeDetails;

	// Assign details to struct.
	TreeDetails.ID = ID;
	TreeDetails.Transform = GetActorTransform();

	// Save details as struct to communicator. Which will be used during saving.
	Communicator::GetInstance().SpawnedGatherableTreeDetails.Add(TreeDetails);
}

void AGatherableTree::EmptyCommunicatorDetailsArray()
{
	Communicator::GetInstance().SpawnedGatherableTreeDetails.Empty();
}

void AGatherableTree::SpawnLoadedActors()
{
	/* Spawn previously saved characters from savefile. */

	FActorSpawnParameters SpawnParams;

	static AObjectFactory* Factory = NewObject<AObjectFactory>();

	// Get actor details to spawn from communicator.
	//auto ActorToSpawn = Communicator::GetInstance().GatherableTreeBlueprint;

	// Iterate over array and saved spawn actors.
	for (auto Details : Communicator::GetInstance().SpawnedGatherableTreeDetails)
	{

		auto ObjectToPlace = Factory->CreateObjectBetter(Details.ID);
		auto ClassToSpawn = ObjectToPlace->GetClass();

		FTransform ActorTransform = Details.Transform;
		AGatherableTree* SpawnedItem = (AGatherableTree*)Communicator::GetInstance().World->SpawnActor<AGatherableTree>(ClassToSpawn, ActorTransform, SpawnParams);

		SpawnedItem->SetupType(Factory->GetObjectNameFromID(Details.ID));
		SpawnedItem->SetMeshToDefault();
	}
}

