// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingStation.h"
#include "Settlement.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "StoneAgeColonyCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

ACraftingStation::ACraftingStation(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> PropertiesDataObject(TEXT("DataTable'/Game/Uluc/DataTables/CraftingStationsDataTable.CraftingStationsDataTable'"));
	if (PropertiesDataObject.Succeeded())
	{
		PropertiesDataTable = PropertiesDataObject.Object;
	}

	DefaultMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MultistoryDungeons/Meshes/Props/Table_Wooden_01.Table_Wooden_01"));

	UE_LOG(LogTemp, Warning, TEXT("ACraftingStation::ACraftingStation"));
}

void ACraftingStation::OnUsed(APawn* InstigatorPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("Im a crafting station"));

	OpenMenu(InstigatorPawn); // CURRENTLY CRASHES
	//((AStoneAgeColonyCharacter*)InstigatorPawn)->AddToInventory(0, 1);
}

void ACraftingStation::OpenMenu(APawn* InstigatorPawn)
{
	((AStoneAgeColonyCharacter*)InstigatorPawn)->OpenMenu(Data->Menu);
	MenuOpen = true;
}

void ACraftingStation::SetupType(FString Type)
{
	UE_LOG(LogTemp, Warning, TEXT("ACraftingStation::SetupType"));
	CraftingStationType = FName(*Type);
	
	const FString ContextString(TEXT("Edible Type Context"));
	Data = PropertiesDataTable->FindRow<FCraftingStationData>(CraftingStationType, ContextString, true);
	ID = Data->ID;
	CraftRequirements = Data->CraftRequirements;
	CraftableItems = Data->CraftableItems;
	// Required for loading icon from TAssetPtr with Get()
	if (Data->Icon.IsPending())
	{
		UAssetManager* tmp = NewObject<UAssetManager>();
		FStreamableManager& AssetMgr = tmp->GetStreamableManager(); //UAssetManager::GetStreamableManager();
		const FStringAssetReference& IconRef = Data->Icon.ToStringReference();
		Data->Icon = Cast<UTexture2D>(AssetMgr.SynchronousLoad(IconRef));

		const FStringAssetReference& MeshRef = Data->Mesh.ToStringReference();
		Data->Mesh = Cast<UStaticMesh>(AssetMgr.SynchronousLoad(MeshRef));



		//const FStringAssetReference& MenuRef = Data->Menu.ToStringReference();
		//Data->Menu = Cast<UTexture2D>(AssetMgr.SynchronousLoad(MenuRef));
	}

	InventoryTexture = Data->Icon.Get();
	DefaultMesh = Data->Mesh.Get();
	//Menu = Data->Menu.Get();
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
				UE_LOG(LogTemp, Warning, TEXT("ACraftingStation::OnOverlapEnd ---> DeRegisterStructure"));
			}


		}
	}
}