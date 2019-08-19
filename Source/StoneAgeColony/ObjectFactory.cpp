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

//AObjectFactory* AObjectFactory::instance;

// Sets default values
AObjectFactory::AObjectFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//this->AddToRoot();

	// ID to Name Table
	static ConstructorHelpers::FObjectFinder<UDataTable> IDtoNameTableObject(TEXT("DataTable'/Game/Uluc/DataTables/ObjectNameDataTable.ObjectNameDataTable'"));
	if (IDtoNameTableObject.Succeeded())
	{
		IDtoNameTable = IDtoNameTableObject.Object;
		
	}

	//// Edibles Table
	//static ConstructorHelpers::FObjectFinder<UDataTable> EdiblesTableObject(TEXT("DataTable'/Game/Uluc/DataTables/EdiblesDataTable.EdiblesDataTable'"));
	//if (EdiblesTableObject.Succeeded())
	//{
	//	EdiblesTable = EdiblesTableObject.Object;
	//}

	//// Equipment Table
	//static ConstructorHelpers::FObjectFinder<UDataTable> EquipmentTableObject(TEXT("DataTable'/Game/Uluc/DataTables/EquipmentsDataTable.EquipmentsDataTable'"));
	//if (EquipmentTableObject.Succeeded())
	//{
	//	UDataTable* EquipmentTable = EquipmentTableObject.Object;
	//	ClassToTable.Emplace(UEquipment::StaticClass(), EquipmentTable);
	//}

}

//AObjectFactory* AObjectFactory::GetInstance()
//{
//	if (!instance)
//	{
//		instance = NewObject<AObjectFactory>();
//	}
//	return instance;
//}

template <typename T>
T* AObjectFactory::CreateObject(int32 ObjectID)
{
	/* Created object and does sets up object from ID */
	FString Tmp = FString::FromInt(ObjectID);
	FName ObjectID_ = FName(*Tmp);

	// Get ObjectName from tables
	const FString ContextString(TEXT("Object Type Context"));
	auto Data = IDtoNameTable->FindRow<FObjectNameData>(ObjectID_, ContextString, true);
	auto ObjectName = Data->Name;

	//auto RelevantTable = ClassToTable[T::StaticClass()];
	
	// Create object and set it up
	T* CreatedObject = NewObject<T>();
	CreatedObject->SetupType(ObjectName);
	
	UE_LOG(LogTemp, Warning, TEXT("AObjectFactory::CreateObject ObjectName: %s"), *ObjectName);
	return CreatedObject;
}

AUsableActor* AObjectFactory::CreateObjectBetter(int32 ObjectID)
{
	AUsableActor* ObjectToReturn = nullptr;

	
	// Get ObjectName from tables
	FString Tmp = FString::FromInt(ObjectID);
	FName ObjectID_ = FName(*Tmp);
	const FString ContextString(TEXT("Object Type Context"));
	auto Data = IDtoNameTable->FindRow<FObjectNameData>(ObjectID_, ContextString, true);
	auto ObjectName = Data->Name;

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

	ObjectToReturn->SetupType(ObjectName);
	UE_LOG(LogTemp, Warning, TEXT("AObjectFactory::CreateObject ObjectName: %s"), *ObjectName);
	return ObjectToReturn;
}

