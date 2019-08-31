// Fill out your copyright notice in the Description page of Project Settings.

#include "Edible.h"
#include "StoneAgeColonyCharacter.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"

AEdible::AEdible(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> PropertiesDataObject(TEXT("DataTable'/Game/Uluc/DataTables/EdiblesDataTable.EdiblesDataTable'"));
	if (PropertiesDataObject.Succeeded())
	{
		PropertiesDataTable = PropertiesDataObject.Object;
	}

	//// Set default edible type to prevent crashes if setup being forgotten
	//SetupEdibleType("Apple");
	
}

void AEdible::SetupType(FString Type)
{

	UE_LOG(LogTemp, Warning, TEXT("AEdible::SetupType"));
	EdibleType = FName(*Type);

	const FString ContextString(TEXT("Edible Type Context"));
	Data = PropertiesDataTable->FindRow<FEdibleData>(EdibleType, ContextString, true);
	ID = Data->ID;
	Description = Data->Description;

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

void AEdible::OnUsed(APawn* InstigatorPawn)
{
	((AStoneAgeColonyCharacter*)InstigatorPawn)->Gold += Data->Health;
}

void AEdible::OnUsedFromInventory(APawn* InstigatorPawn)
{
	((AStoneAgeColonyCharacter*)InstigatorPawn)->Gold += Data->Health;
}

void AEdible::test()
{
	UE_LOG(LogTemp, Warning, TEXT("EDIBLE ROW DATA: %d"), Data->Health)
}