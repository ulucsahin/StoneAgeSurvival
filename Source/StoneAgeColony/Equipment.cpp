// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
AEquipment::AEquipment(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> PropertiesDataObject(TEXT("DataTable'/Game/Uluc/DataTables/EquipmentsDataTable.EquipmentsDataTable'"));
	if (PropertiesDataObject.Succeeded())
	{
		PropertiesDataTable = PropertiesDataObject.Object;
		UE_LOG(LogTemp,Warning,TEXT("UEquipment::UEquipment PropertiesDataObject.Succeeded"))
	}
}

void AEquipment::SetupType(FString ItemName)
{
	UE_LOG(LogTemp, Warning, TEXT("UEquipment::SetupType"));

	const FString ContextString(TEXT("Equipment Type Context"));
	auto Data = PropertiesDataTable->FindRow<FEquipmentData>(FName(*ItemName), ContextString, true);
	ID = Data->ID;
	EquipmentType = TypeMap[Data->Type];
	Stat1 = Data->Stat1;

	// Required for loading icon from TAssetPtr with Get()
	if (Data->MeshRef.IsPending())
	{
		UAssetManager* tmp = NewObject<UAssetManager>();
		FStreamableManager& AssetMgr = tmp->GetStreamableManager();//UAssetManager::GetStreamableManager();
		const FStringAssetReference& AssetRef = Data->MeshRef.ToStringReference();
		Data->MeshRef = Cast<USkeletalMesh>(AssetMgr.SynchronousLoad(AssetRef));
	}

	SkeletalMesh = Data->MeshRef.Get();
	
}
