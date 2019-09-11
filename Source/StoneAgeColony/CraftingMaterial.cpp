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

}

void ACraftingMaterial::SetupType(FString Name)
{
	MaterialType = FName(*Name);

	const FString ContextString(TEXT("Edible Type Context"));
	Data = PropertiesDataTable->FindRow<FCraftingMaterialData>(MaterialType, ContextString, true);
	ID = Data->ID;
	YieldAmount = Data->YieldAmount;
}