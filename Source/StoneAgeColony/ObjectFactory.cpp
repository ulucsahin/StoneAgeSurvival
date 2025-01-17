// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectFactory.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"


// Objects that will be spawned by ObjectFactory
#include "Equipment.h"
#include "GatherableTree.h"
#include "TestGameLoader.h"
#include "PeopleSpawner.h"
#include "ObjectBed.h"
#include "Settlement.h"
#include "Edible.h"
#include "CraftingStation.h"
#include "House.h"
#include "UsableActor.h"
#include "CraftingMaterial.h"
#include "Farm.h"
#include "Plant.h"
#include "Communicator.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
//AObjectFactory* AObjectFactory::instance;

// Sets default values
AObjectFactory::AObjectFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// ID to Name Table
	static ConstructorHelpers::FObjectFinder<UDataTable> CommonPropertiesTableObject(TEXT("DataTable'/Game/Uluc/DataTables/ObjectsCommonPropertiesDataTable.ObjectsCommonPropertiesDataTable'"));
	if (CommonPropertiesTableObject.Succeeded())
	{
		CommonPropertiesTable = CommonPropertiesTableObject.Object;
		
	}

}

template <typename T>
T* AObjectFactory::CreateObject(int32 ObjectID)
{
	/* Created object and does sets up object from ID */
	FString Tmp = FString::FromInt(ObjectID);
	FName ObjectID_ = FName(*Tmp);

	// Get ObjectName from tables
	const FString ContextString(TEXT("Object Type Context"));
	auto Data = CommonPropertiesTable->FindRow<FObjectNameData>(ObjectID_, ContextString, true);
	auto ObjectName = Data->Name;

	//auto RelevantTable = ClassToTable[T::StaticClass()];
	
	// Create object and set it up
	T* CreatedObject = NewObject<T>();
	CreatedObject->SetupType(ObjectName);
	
	return CreatedObject;
}

AUsableActor* AObjectFactory::CreateObjectBetter(int32 ObjectID)
{
	AUsableActor* ObjectToReturn = nullptr;

	// Get ObjectName from tables
	FString Tmp = FString::FromInt(ObjectID);
	FName ObjectID_ = FName(*Tmp);
	const FString ContextString(TEXT("Object Type Context"));
	//UE_LOG(LogTemp, Warning, TEXT("AObjectFactory:: CreateObjectBetter Object ID: %s"), *Tmp);
	auto Data = CommonPropertiesTable->FindRow<FObjectCommonPropertiesData>(ObjectID_, ContextString, true);
	auto ObjectName = Data->Name_;

	// Unique Items, will remove later?
	if (ObjectID == 0)
	{
		ObjectToReturn = NewObject<AUsableActor>();
	}
	else if (ObjectID == 1)
	{
		ObjectToReturn = NewObject<ATestGameLoader>();
	}
	else if (ObjectID == 2)
	{
		ObjectToReturn = NewObject<APeopleSpawner>();
	}
	else if (ObjectID == 3)
	{
		ObjectToReturn = NewObject<AObjectBed>();
	}
	// Gatherables (tree, rock, etc.)
	else if (ObjectID >= 100 && ObjectID <= 199)
	{
		ObjectToReturn = NewObject<AGatherableTree>(); // TODO: Gatherable tree for now, will need to change later.
		//TODO: Will need to seperate materials from sources, for example: gathering trees will yield wood.
	}
	// Edibles
	else if (ObjectID >= 200 && ObjectID <= 299)
	{
		ObjectToReturn = NewObject<AEdible>();
	}
	// Crafting Stations, wall is a crafting station aswell 
	else if (ObjectID >= 400 && ObjectID <= 449)
	{
		ObjectToReturn = NewObject<ACraftingStation>();
	}
	// Farms
	else if (ObjectID >= 450 && ObjectID <= 489)
	{
		ObjectToReturn = NewObject<AFarm>();
	}
	// Houses
	else if (ObjectID >= 490 && ObjectID <= 499)
	{
		ObjectToReturn = NewObject<AHouse>();
	}
	// CraftingMaterials
	else if(ObjectID >= 500 && ObjectID <= 699)
	{
		ObjectToReturn = NewObject<ACraftingMaterial>();
	}
	// Plants
	else if (ObjectID >= 700 && ObjectID <= 799)
	{
		ObjectToReturn = NewObject<APlant>();
	}
	// Equipment
	else if (ObjectID >= 1000 && ObjectID <= 1199)
	{
		ObjectToReturn = NewObject<AEquipment>();
	}
	// Settlement
	else if (ObjectID == 10000)
	{
		ObjectToReturn = NewObject<ASettlement>();
	}
	
	// Set common properties of created object
	ObjectToReturn->ID = ObjectID;
	ObjectToReturn->Description = Data->Description;
	ObjectToReturn->CraftRequirements = Data->CraftRequirements;
	ObjectToReturn->SpecialID = Communicator::GetInstance().GenerateID();
	
	if (Data->Icon.IsPending())
	{
		UAssetManager* tmp = NewObject<UAssetManager>();
		FStreamableManager& AssetMgr = tmp->GetStreamableManager(); //UAssetManager::GetStreamableManager();
		const FStringAssetReference& IconRef = Data->Icon.ToStringReference();
		Data->Icon = Cast<UTexture2D>(AssetMgr.SynchronousLoad(IconRef));
	}
	ObjectToReturn->InventoryTexture = Data->Icon.Get();

	// Set non-common properties of created object
	ObjectToReturn->SetupType(ObjectName);

	return ObjectToReturn;
}

FString AObjectFactory::GetObjectNameFromID(int32 ObjectID)
{
	// Get ObjectName from tables
	FString Tmp = FString::FromInt(ObjectID);
	FName ObjectID_ = FName(*Tmp);
	const FString ContextString(TEXT("Object Type Context"));
	auto Data = CommonPropertiesTable->FindRow<FObjectCommonPropertiesData>(ObjectID_, ContextString, true);
	auto ObjectName = Data->Name_;

	return ObjectName;
}