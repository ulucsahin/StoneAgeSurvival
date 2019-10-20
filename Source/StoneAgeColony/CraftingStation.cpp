// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingStation.h"
#include "Settlement.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "StoneAgeColonyCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "SurvivalWidget.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "CraftingStationMenu.h"
#include "StoneAgeColonyCharacter.h"
#include "HumanCharacter.h"
#include "Inventory.h"
#include "ObjectFactory.h"
#include "Communicator.h"

ACraftingStation::ACraftingStation(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> PropertiesDataObject(TEXT("DataTable'/Game/Uluc/DataTables/CraftingStationsDataTable.CraftingStationsDataTable'"));
	if (PropertiesDataObject.Succeeded())
	{
		PropertiesDataTable = PropertiesDataObject.Object;
	}

	CraftAmount = 1; // TODO: craft how many?
	
}

void ACraftingStation::OnUsed(APawn* InstigatorPawn)
{
	/* When player interacts, not for NPCs*/
	Super::OnUsed(InstigatorPawn);
	Player = (AStoneAgeColonyCharacter*)InstigatorPawn;
	OpenMenu(InstigatorPawn);
}

void ACraftingStation::OpenMenu(APawn* InstigatorPawn)
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

		// Set bar visible if currently crafting
		((UCraftingStationMenu*)Menu)->SetProgressBarVisibility(CurrentlyCrafting); 
		MenuOpen = true;
	}
	else
	{
		if (!Menu->IsActive)
		{
			Menu = ((AStoneAgeColonyCharacter*)InstigatorPawn)->OpenMenu(MenuRef, this, OwnerSettlement);

			// Set bar visible if currently crafting
			((UCraftingStationMenu*)Menu)->SetProgressBarVisibility(CurrentlyCrafting);
			MenuOpen = true;
		}
	}
}

void ACraftingStation::SetupType(FString Type)
{
	CraftingStationType = FName(*Type);
	Name = CraftingStationType;

	const FString ContextString(TEXT("Edible Type Context"));
	Data = PropertiesDataTable->FindRow<FStructureData>(CraftingStationType, ContextString, true);
	ID = Data->ID;
	CraftableItems = Data->CraftableItems;
	MenuRef = Data->Menu;
	
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

void ACraftingStation::StartCrafting(float CraftingTime, APawn* InstigatorPawn)
{
	/* Set the character that does the crafting. Can be player or npcs. */
	CraftingCharacter = Cast<AHumanCharacter>(InstigatorPawn);

	((UCraftingStationMenu*)Menu)->SetProgressBarVisibility(true);
	FTimerDelegate TimerDel;

	//Binding the function with specific values
	float UpdateFrequency = 0.1f;
	TimerDel.BindUFunction(this, FName("CraftingStep"), CraftingTime, UpdateFrequency);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 0.1f, true);

	CurrentlyCrafting = true;

}

void ACraftingStation::CraftingStep(float CraftingTime, float UpdateFrequency)
{
	CraftingProgress += 1 / CraftingTime * UpdateFrequency;
	((UCraftingStationMenu*)Menu)->UpdateProgressBar(CraftingProgress);
	
	// Crafting Completed
	if (CraftingProgress >= 1.f)
	{
		StopCrafting();

		// Consume items from player inventory after crafting is finished.
		if (Player)
		{
			auto PlayerInventory = Player->GetInventory();

			// Consume items from player inventory
			for (auto Requirement : CurrentItem->CraftRequirements)
			{
				int32 UsedItemID = Requirement.Key;
				int32 ConsumedAmount = Requirement.Value * CraftAmount;
				Player->ConsumeItemFromInventory(UsedItemID, ConsumedAmount);
			
			}

			// Add crafted items to player inventory
			Player->Inventory->AddItem(CurrentItemID, CraftAmount * CurrentItem->YieldAmount);
			
		}

	}
}

void ACraftingStation::StopCrafting()
{
	((UCraftingStationMenu*)Menu)->SetProgressBarVisibility(false);
	CraftingProgress = 0.f;
	CurrentlyCrafting = false;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

bool ACraftingStation::CraftingRequirementsMet()
{
	/* Checks if required items are in player inventory for crafting the item of this button. */

	bool RequirementsMet = true;

	if (Player)
	{
		auto PlayerInventory = Player->GetInventory();

		for (auto Requirement : CurrentItem->CraftRequirements)
		{
			int32 RequiredItem = Requirement.Key;
			int32 RequiredAmount = Requirement.Value * CraftAmount;

			// if player don't have enough of this item
			if (PlayerInventory->Contains(RequiredItem))
			{
				if (PlayerInventory->GetItems()[RequiredItem] < RequiredAmount)
				{
					RequirementsMet = false;
					break;
				}
			}
			else
			{
				RequirementsMet = false;
			}

		}

	}

	return RequirementsMet;
}


void ACraftingStation::RegisterActorDetailsToSave()
{
	FCraftingStationDetails Details;
	
	Details.ID = ID;
	Details.SpecialID = SpecialID;
	Details.Transform = GetActorTransform();

	Communicator::GetInstance().SpawnedCraftingStationDetails.Add(Details);
}

void ACraftingStation::EmptyCommunicatorDetailsArray()
{
	Communicator::GetInstance().SpawnedCraftingStationDetails.Empty();
}

void ACraftingStation::SpawnLoadedActors()
{
	/* Spawn previously saved characters from savefile. */
	FActorSpawnParameters SpawnParams;

	AObjectFactory* Factory = NewObject<AObjectFactory>();

	// Iterate over array and saved spawn actors.
	for (auto Details : Communicator::GetInstance().SpawnedCraftingStationDetails)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ACraftingStation::SpawnLoadedActors ID: %d"), Details.ID);
		auto ObjectToPlace = Factory->CreateObjectBetter(Details.ID);
		auto ClassToSpawn = ObjectToPlace->GetClass();

		FTransform ActorTransform = Details.Transform;
		ACraftingStation* SpawnedItem = (ACraftingStation*)Communicator::GetInstance().World->SpawnActor<AUsableActor>(ClassToSpawn, ActorTransform, SpawnParams);
		
		SpawnedItem->SetupType(Factory->GetObjectNameFromID(Details.ID));
		SpawnedItem->SetMeshToDefault();
		SpawnedItem->SpecialID = Details.SpecialID;
	}
}