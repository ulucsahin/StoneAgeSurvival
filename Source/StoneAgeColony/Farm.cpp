// Fill out your copyright notice in the Description page of Project Settings.

#include "Farm.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "StoneAgeColonyCharacter.h"
#include "HumanCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ObjectFactory.h"
#include "SurvivalWidget.h"
#include "Plant.h"
#include "UIBottomBar.h"
#include "BottomBarItem.h"
#include "Communicator.h"
#include "Inventory.h"

#include "EngineUtils.h"
//#include "InstancedFoliageActor.h"

AFarm::AFarm(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> PropertiesDataObject(TEXT("DataTable'/Game/Uluc/DataTables/FarmsDataTable.FarmsDataTable'"));
	if (PropertiesDataObject.Succeeded())
	{
		PropertiesDataTable = PropertiesDataObject.Object;
	}

}

void AFarm::SetupType(FString Type)
{
	FarmType = FName(*Type);
	const FString ContextString(TEXT("Farm Type Context"));

	Data = PropertiesDataTable->FindRow<FFarmData>(FarmType, ContextString, true);
	ID = Data->ID;
	PlotCapacity = Data->PlotCapacity; // currently not used
	MenuRef = Data->Menu;

	if (Data->Mesh.IsPending())
	{
		UAssetManager* tmp = NewObject<UAssetManager>();
		FStreamableManager& AssetMgr = tmp->GetStreamableManager();
		const FStringAssetReference& MeshRef = Data->Mesh.ToStringReference();
		Data->Mesh = Cast<UStaticMesh>(AssetMgr.SynchronousLoad(MeshRef));
	}

	MeshComp->SetStaticMesh(Data->Mesh.Get());
	DefaultMesh = MeshComp->GetStaticMesh();

	// initialize SocketFull map
	auto SocketNames = MeshComp->GetAllSocketNames();

	for (auto SocketName_ : SocketNames)
	{
		FString SocketName = SocketName_.ToString();
		SocketFull.Emplace(SocketName, false); // all sockets are empty at the beginning
		PlantsInSockets.Emplace(SocketName, nullptr); // all sockets are empty at the beginning
	}

	Player = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Factory = NewObject<AObjectFactory>();
}

void AFarm::OnUsed(APawn* InstigatorPawn)
{
	//UE_LOG(LogTemp, Warning, TEXT("Farm : :Debug"));

	int32 ItemToPlantID = Player->BottomBar->BarItems[Player->BottomBar->SelectedSlot]->ItemID;

	// ID range of plantable objects
	if (ItemToPlantID >= 700 && ItemToPlantID <= 799)
	{
		// Plant on a suitable socket. TODO: Player should choose which socket to plant.
		Plant(ItemToPlantID, SelectSocketToPlant(), false); // plant potato to test
		Player->BottomBar->Refresh();
	}

}

// Currently not used as farms dont have any menus
void AFarm::OpenMenu(APawn* InstigatorPawn)
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
		Menu = ((AStoneAgeColonyCharacter*)InstigatorPawn)->OpenMenu(MenuRef, this, OwnerSettlement);
	}
	else
	{
		if (!Menu->IsActive)
		{
			Menu = ((AStoneAgeColonyCharacter*)InstigatorPawn)->OpenMenu(MenuRef, this, OwnerSettlement);
		}
	}
}

APlant* AFarm::Plant(int32 ItemIDToPlant, FName SocketName, bool FromSave)
{
	/* Plants a plant to socket
	 if FromSave, then don't use item from player inventory
	*/

	if (SocketName.ToString() == "") return nullptr;
	
	FString SocketName_ = SocketName.ToString();
	
	// Plant only if socket is empty
	if (!SocketFull[SocketName_])
	{
		// If not spawning from save, check if player has required seed/item to plant.
		if(!FromSave)
		{
			auto Inventory = CraftingCharacter->GetInventory();
			if (Inventory->Contains(ItemIDToPlant)) // sometimes this returns true if player has 0 of that item (we accept it as false so we check item amount)
			{
				if (Inventory->Items[ItemIDToPlant] > 0)
				{
					CraftingCharacter->Inventory->ConsumeItem(ItemIDToPlant, 1); // consume 1 from player inventory
				}
				else
				{
					return nullptr; // if player dont have item dont do anything
				}
			}
		}
		
		// Create object, attach to socket, setup
		APlant* ObjectToPlant = (APlant*)Factory->CreateObjectBetter(ItemIDToPlant);
		auto ObjectName = Factory->GetObjectNameFromID(ItemIDToPlant);
		auto ClassToSpawn = ObjectToPlant->GetClass();
		auto SpawnedItem = GetWorld()->SpawnActor<APlant>(ClassToSpawn, GetActorLocation(), FRotator::ZeroRotator);
		SpawnedItem->SetupType(ObjectName);
		SpawnedItem->SetMeshToDefault();
		SpawnedItem->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
		RandomizePlantAppearance(SpawnedItem);
		SpawnedItem->bIsPickupable = false;
		SpawnedItem->StartGrowing();
		SpawnedItem->OwnerFarm = this;
		SpawnedItem->OwnerSocket = SocketName_;

		SocketFull[SocketName_] = true; // mark socket as planted
		PlantsInSockets.Emplace(SocketName_, SpawnedItem);
		return SpawnedItem;
	}
	else
	{
		return nullptr;
	}

	
}

void AFarm::RemovePlant(FString SocketName)
{
	/* Removes plant from socket name */

	// destroy actor and mark sockets as empty
	PlantsInSockets[SocketName]->Destroy();
	SocketFull[SocketName] = false;
	PlantsInSockets.Emplace(SocketName, nullptr);
}

void AFarm::RandomizePlantAppearance(APlant* Plant)
{
	// Set Random Rotation, must be done after attaching to socket.
	auto Rot = Plant->GetActorRotation();
	float RandomYaw = FMath::RandRange(0.f, 360.f);
	Rot.Yaw += RandomYaw;
	Plant->SetActorRotation(Rot);

	// Set Random Scale
	Plant->SetActorScale3D(FVector(FMath::RandRange(0.8f, 1.2f), FMath::RandRange(0.8f, 1.2f), FMath::RandRange(0.8f, 1.2f)));
}

FName AFarm::SelectSocketToPlant()
{
	/* Selects closest socket where player is looking. 
	 * Mostly copied from BuildingManager::CalculateMinDistanceSocket
	*/

	FVector CamLoc;
	FRotator CamRot;
	Player->Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	int32 InteractDistance = 350;
	const FVector TraceEnd = TraceStart + (Direction * InteractDistance);
	auto Sockets = MeshComp->GetAllSocketNames();
	float SmallestDistance = 999999.f;
	FName ClosestSocketName = "";

	if (Sockets.Num() > 0)
	{
		for (auto temp : Sockets)
		{
			FString SocketName = temp.ToString();
			UE_LOG(LogTemp, Warning, TEXT("SocketName %s"), *SocketName);
			FVector SocketLocation = MeshComp->GetSocketLocation(*SocketName);
			float Distance = FVector::Dist(TraceEnd, SocketLocation);

			if (Distance < SmallestDistance)
			{
				SmallestDistance = Distance;		
				ClosestSocketName = temp;
			}
		}
	}

	return ClosestSocketName;
}

//
// Save-Load Methods below
//

void AFarm::RegisterActorDetailsToSave()
{
	FFarmDetails Details;

	Details.ID = ID;
	Details.Transform = GetActorTransform();

	// Save Details of plants in sockets so we can recreate them upon loading.
	Details.PlantDetailsInSockets.Empty();
	for (auto Item : PlantsInSockets)
	{
		auto Plant = (APlant*)Item.Value;
		if (Plant)
		{
			Details.PlantDetailsInSockets.Emplace(Item.Key, Plant->GetDetails());
		}
		
	}

	Communicator::GetInstance().SpawnedFarmDetails.Add(Details);
}

void AFarm::EmptyCommunicatorDetailsArray()
{
	Communicator::GetInstance().SpawnedFarmDetails.Empty();
}

void AFarm::SpawnLoadedActors()
{
	/* Spawn previously saved characters from savefile. */
	FActorSpawnParameters SpawnParams;

	AObjectFactory* Factory = NewObject<AObjectFactory>();

	// Iterate over array and saved spawn actors.
	for (auto Details : Communicator::GetInstance().SpawnedFarmDetails)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ACraftingStation::SpawnLoadedActors ID: %d"), Details.ID);
		auto ObjectToPlace = Factory->CreateObjectBetter(Details.ID);
		auto ClassToSpawn = ObjectToPlace->GetClass();

		FTransform ActorTransform = Details.Transform;
		AFarm* SpawnedItem = (AFarm*)Communicator::GetInstance().World->SpawnActor<AUsableActor>(ClassToSpawn, ActorTransform, SpawnParams);

		SpawnedItem->SetupType(Factory->GetObjectNameFromID(Details.ID));
		SpawnedItem->SetMeshToDefault();


		// Spawn plants for current farm.
		FActorSpawnParameters SpawnParams2;
		for (auto Socket_PlantDetails : Details.PlantDetailsInSockets)
		{
			auto PlantDetails = Socket_PlantDetails.Value;
			auto SpawnedPlant = SpawnedItem->Plant(PlantDetails.ID, FName(*Socket_PlantDetails.Key), true);
			
			if (SpawnedPlant)
			{
				SpawnedPlant->CurrentStage = PlantDetails.GrowStage;
				SpawnedPlant->ProgressToNextStage = PlantDetails.GrowProgress;
				SpawnedPlant->ApplyDetails();
			}

		}

	}

}