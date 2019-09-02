// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingMaterial.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"

ACraftingMaterial::ACraftingMaterial(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> PropertiesDataObject(TEXT("DataTable'/Game/Uluc/DataTables/CraftingMaterialsDataTable.CraftingMaterialsDataTable'"));
	if (PropertiesDataObject.Succeeded())
	{
		PropertiesDataTable = PropertiesDataObject.Object;
	}

	//// Set default edible type to prevent crashes if setup being forgotten
	//SetupEdibleType("Apple");

}

void ACraftingMaterial::SetupType(FString Name)
{
	UE_LOG(LogTemp, Warning, TEXT("ACraftingMaterial::SetupType"));
	MaterialType = FName(*Name);

	const FString ContextString(TEXT("Edible Type Context"));
	UE_LOG(LogTemp, Warning, TEXT("ACraftingMaterial::SetupType Material Type: %s"), *Name);
	Data = PropertiesDataTable->FindRow<FCraftingMaterialData>(MaterialType, ContextString, true);
	ID = Data->ID;
	Description = Data->Description;
	CraftRequirements = Data->CraftRequirements;

	// Required for loading icon from TAssetPtr with Get()
	if (Data->Icon.IsPending())
	{
		UAssetManager* tmp = NewObject<UAssetManager>();
		FStreamableManager& AssetMgr = tmp->GetStreamableManager();//UAssetManager::GetStreamableManager();
		const FStringAssetReference& AssetRef = Data->Icon.ToStringReference();
		Data->Icon = Cast<UTexture2D>(AssetMgr.SynchronousLoad(AssetRef));
	}

	InventoryTexture = Data->Icon.Get();
}