// Fill out your copyright notice in the Description page of Project Settings.

#include "House.h"

void AHouse::SetupType(FString Type)
{
	HouseType = FName(*Type);
	const FString ContextString(TEXT("Edible Type Context"));

	//BURADAN DEVAM ET Data structure oluşturmak lazım houselar icin, crafting station gibi aynı. sonra objectfactoryde if ekle falan.


	//Data = PropertiesDataTable->FindRow<FCraftingStationData>(CraftingStationType, ContextString, true);
	//ID = Data->ID;
	//CraftRequirements = Data->CraftRequirements;
	//CraftableItems = Data->CraftableItems;
	//Description = Data->Description;

	//// Required for loading icon from TAssetPtr with Get()
	//if (Data->Icon.IsPending())
	//{
	//	UAssetManager* tmp = NewObject<UAssetManager>();
	//	FStreamableManager& AssetMgr = tmp->GetStreamableManager(); //UAssetManager::GetStreamableManager();
	//	const FStringAssetReference& IconRef = Data->Icon.ToStringReference();
	//	Data->Icon = Cast<UTexture2D>(AssetMgr.SynchronousLoad(IconRef));

	//	const FStringAssetReference& MeshRef = Data->Mesh.ToStringReference();
	//	Data->Mesh = Cast<UStaticMesh>(AssetMgr.SynchronousLoad(MeshRef));
	//}

	//InventoryTexture = Data->Icon.Get();
	//DefaultMesh = Data->Mesh.Get();
}