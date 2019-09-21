// Fill out your copyright notice in the Description page of Project Settings.

#include "Farm.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "StoneAgeColonyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ObjectFactory.h"
#include "SurvivalWidget.h"
#include "Plant.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"

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
	PlotCapacity = Data->PlotCapacity;
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


	Factory = NewObject<AObjectFactory>();
}

void AFarm::OnUsed(APawn* InstigatorPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("Farm used"));
	//OpenMenu(InstigatorPawn);
	Plant(700, "Plot1");
}

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

void AFarm::Plant(int32 ItemIDToPlant, FName SocketName)
{
	/* Plants a plant to socket
	*/

	FString SocketName_ = SocketName.ToString();

	// Plant only if socket is empty
	if (!SocketFull[SocketName_])
	{
		APlant* ObjectToPlant = (APlant*)Factory->CreateObjectBetter(ItemIDToPlant);
		auto ObjectName = Factory->GetObjectNameFromID(ItemIDToPlant);
		auto ClassToSpawn = ObjectToPlant->GetClass();
		auto SpawnedItem = GetWorld()->SpawnActor<APlant>(ClassToSpawn, GetActorLocation(), FRotator::ZeroRotator);
		SpawnedItem->SetupType(ObjectName);
		SpawnedItem->SetMeshToDefault();
		SpawnedItem->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
		SpawnedItem->bIsPickupable = false;
		SpawnedItem->StartGrowing();
		SpawnedItem->OwnerFarm = this;
		SpawnedItem->OwnerSocket = SocketName_;
		SocketFull[SocketName_] = true; // mark socket as planted
		PlantsInSockets.Emplace(SocketName_, SpawnedItem);
	}

}

void AFarm::RemovePlant(FString SocketName)
{
	/* Removes plant from socket name */

	//FString SocketName_ = SocketName.ToString();

	// destroy actor and mark sockets as empty
	PlantsInSockets[SocketName]->Destroy();
	SocketFull[SocketName] = false;
	PlantsInSockets.Emplace(SocketName, nullptr);
}