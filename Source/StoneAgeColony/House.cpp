// Fill out your copyright notice in the Description page of Project Settings.

#include "House.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "StoneAgeColonyCharacter.h"
#include "SurvivalWidget.h"
#include "Communicator.h"
#include "ObjectFactory.h"

AHouse::AHouse(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> PropertiesDataObject(TEXT("DataTable'/Game/Uluc/DataTables/HousesDataTable.HousesDataTable'"));
	if (PropertiesDataObject.Succeeded())
	{
		PropertiesDataTable = PropertiesDataObject.Object;
	}
}

void AHouse::OnUsed(APawn* InstigatorPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("in da hosuze"));
	OpenMenu(InstigatorPawn);
}

void AHouse::OpenMenu(APawn* InstigatorPawn)
{
	/* Prevents opening multiple of same menus */

	// Data->Menu is empty if this station has no menu.
	if (Data->Menu == "")
	{
		return;
	}

	// Checks if menu is already open or not.
	if (!Menu)
	{
		Menu = ((AStoneAgeColonyCharacter*)InstigatorPawn)->OpenMenu(Data->Menu, this, OwnerSettlement);
		
	}
	else
	{
		if (!Menu->IsActive)
		{
			Menu = ((AStoneAgeColonyCharacter*)InstigatorPawn)->OpenMenu(Data->Menu, this, OwnerSettlement);
		}
	}
}

void AHouse::SetupType(FString Type)
{
	HouseType = FName(*Type);
	const FString ContextString(TEXT("House Type Context"));

	Data = PropertiesDataTable->FindRow<FHouseData>(HouseType, ContextString, true);
	ID = Data->ID;
	Capacity = Data->Capacity;

	if (Data->Mesh.IsPending())
	{
		UAssetManager* tmp = NewObject<UAssetManager>();
		FStreamableManager& AssetMgr = tmp->GetStreamableManager();
		const FStringAssetReference& MeshRef = Data->Mesh.ToStringReference();
		Data->Mesh = Cast<UStaticMesh>(AssetMgr.SynchronousLoad(MeshRef));
	}

	DefaultMesh = Data->Mesh.Get();
}

void AHouse::RegisterActorDetailsToSave()
{
	UE_LOG(LogTemp, Warning, TEXT("AHouse::RegisterActorDetailsToSave"));
	FHouseDetails Details;

	Details.ID = ID;
	Details.Transform = GetActorTransform();
	Details.Capacity = Capacity;

	Communicator::GetInstance().SpawnedHouseDetails.Add(Details);
}

void AHouse::EmptyCommunicatorDetailsArray()
{
	UE_LOG(LogTemp, Warning, TEXT("AHouse::EmptyCommunicatorDetailsArray"));
	Communicator::GetInstance().SpawnedHouseDetails.Empty();
}

void AHouse::SpawnLoadedActors()
{
	UE_LOG(LogTemp, Warning, TEXT("AHouse::SpawnLoadedActors"));
	/* Spawn previously saved characters from savefile. */
	FActorSpawnParameters SpawnParams;

	static AObjectFactory* Factory = NewObject<AObjectFactory>();

	// Iterate over array and saved spawn actors.
	for (auto Details : Communicator::GetInstance().SpawnedHouseDetails)
	{
		auto ObjectToPlace = Factory->CreateObjectBetter(Details.ID);
		auto ClassToSpawn = ObjectToPlace->GetClass();

		FTransform ActorTransform = Details.Transform;
		AHouse* SpawnedItem = (AHouse*)Communicator::GetInstance().World->SpawnActor<AUsableActor>(ClassToSpawn, ActorTransform, SpawnParams);

		SpawnedItem->SetupType(Factory->GetObjectNameFromID(Details.ID));
		SpawnedItem->SetMeshToDefault();

		SpawnedItem->Capacity = Details.Capacity;

	}
}