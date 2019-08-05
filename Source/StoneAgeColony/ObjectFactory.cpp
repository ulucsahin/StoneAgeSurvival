// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectFactory.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"


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

//// Called when the game starts or when spawned
//void AObjectFactory::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//// Called every frame
//void AObjectFactory::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

