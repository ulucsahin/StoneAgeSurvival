// Fill out your copyright notice in the Description page of Project Settings.

#include "Farm.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"

AFarm::AFarm(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> PropertiesDataObject(TEXT("DataTable'/Game/Uluc/DataTables/FarmsDataTable.FarmsDataTable'"));
	if (PropertiesDataObject.Succeeded())
	{
		PropertiesDataTable = PropertiesDataObject.Object;
	}
}

void AFarm::SetupType(FString Type)
{
	FarmType = FName(*Type);
	const FString ContextString(TEXT("Farm Type Context"));

	Data = PropertiesDataTable->FindRow<FFarmData>(FarmType, ContextString, true);
	ID = Data->ID;
	PlotCapacity = Data->PlotCapacity;

	if (Data->Mesh.IsPending())
	{
		UAssetManager* tmp = NewObject<UAssetManager>();
		FStreamableManager& AssetMgr = tmp->GetStreamableManager();
		const FStringAssetReference& MeshRef = Data->Mesh.ToStringReference();
		Data->Mesh = Cast<UStaticMesh>(AssetMgr.SynchronousLoad(MeshRef));
	}

	DefaultMesh = Data->Mesh.Get();
}

void AFarm::OnUsed(APawn* InstigatorPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("Im a farm"));
}