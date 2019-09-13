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

ACraftingStation::ACraftingStation(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> PropertiesDataObject(TEXT("DataTable'/Game/Uluc/DataTables/StructuresDataTable.StructuresDataTable'"));
	if (PropertiesDataObject.Succeeded())
	{
		PropertiesDataTable = PropertiesDataObject.Object;
	}

	CraftAmount = 1; // TODO: craft how many?
}

void ACraftingStation::OnUsed(APawn* InstigatorPawn)
{
	Player = (AStoneAgeColonyCharacter*)InstigatorPawn;
	OpenMenu(InstigatorPawn);
}

void ACraftingStation::OpenMenu(APawn* InstigatorPawn)
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
		Menu = ((AStoneAgeColonyCharacter*)InstigatorPawn)->OpenMenu(Data->Menu, this, nullptr); //owner settlement is null for now

		// Set bar visible if currently crafting
		((UCraftingStationMenu*)Menu)->SetProgressBarVisibility(CurrentlyCrafting); 
		MenuOpen = true;
	}
	else
	{
		if (!Menu->IsActive)
		{
			Menu = ((AStoneAgeColonyCharacter*)InstigatorPawn)->OpenMenu(Data->Menu, this, nullptr); //owner settlement is null for now

			// Set bar visible if currently crafting
			((UCraftingStationMenu*)Menu)->SetProgressBarVisibility(CurrentlyCrafting);
			MenuOpen = true;
		}
	}
}

void ACraftingStation::SetupType(FString Type)
{
	CraftingStationType = FName(*Type);
	
	const FString ContextString(TEXT("Edible Type Context"));
	Data = PropertiesDataTable->FindRow<FStructureData>(CraftingStationType, ContextString, true);
	ID = Data->ID;
	CraftableItems = Data->CraftableItems;

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


// NOT CALLED, now handled in settlement OnOverlapBegin
void ACraftingStation::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	//UE_LOG(LogTemp, Warning, TEXT("ACraftingStation::OnOverlapBegin"));
	if (OtherActor != this)
	{
		if (OtherActor != nullptr)
		{
			auto OtherActorCompName = OtherComp->GetFName().ToString(); //OtherActor->GetClass()->GetFName().ToString();
			if (OtherActorCompName == "SettlementArea")
			{
				((ASettlement*)OtherActor)->RegisterStructure(this);
				UE_LOG(LogTemp, Warning, TEXT("ACraftingStation::OnOverlapEnd ----> RegisterStructure"));
			}
			
		}

	}

}

// NOT CALLED, now handled in settlement OnOverlapEnd
void ACraftingStation::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
	//UE_LOG(LogTemp, Warning, TEXT("ACraftingStation::OnOverlapEnd"));
	if (OtherActor != this)
	{
		if (OtherActor != nullptr)
		{
			auto OtherActorCompName = OtherComp->GetFName().ToString(); //OtherActor->GetClass()->GetFName().ToString();
			if (OtherActorCompName == "SettlementArea")
			{
				((ASettlement*)OtherActor)->DeRegisterStructure(this);
				UE_LOG(LogTemp, Warning, TEXT("ACraftingStation::OnOverlapEnd--> DeRegisterStructure"));
			}

		}

	}

}


void ACraftingStation::StartCrafting(float CraftingTime)
{
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
			Player->AddToInventory(CurrentItemID, CraftAmount * CurrentItem->YieldAmount);
			
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
			if (PlayerInventory.Contains(RequiredItem))
			{
				if (PlayerInventory[RequiredItem] < RequiredAmount)
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