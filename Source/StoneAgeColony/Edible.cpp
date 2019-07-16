// Fill out your copyright notice in the Description page of Project Settings.

#include "Edible.h"
#include "StoneAgeColonyCharacter.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

AEdible::AEdible(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> PropertiesDataObject(TEXT("DataTable'/Game/Uluc/DataTables/EdiblesDataTable.EdiblesDataTable'"));
	if (PropertiesDataObject.Succeeded())
	{
		PropertiesDataTable = PropertiesDataObject.Object;
	}

	// Set default edible type to prevent crashes if setup being forgotten
	SetupEdibleType("Apple");
	
}

void AEdible::SetupEdibleType(FString Type)
{
	EdibleType = FName(*Type);

	const FString ContextString(TEXT("Edible Type Context"));
	Data = PropertiesDataTable->FindRow<FEdibleData>(EdibleType, ContextString, true);
}

void AEdible::Use(APawn* InstigatorPawn)
{
	((AStoneAgeColonyCharacter*)InstigatorPawn)->Gold += Data->Health;
}

void AEdible::test()
{
	UE_LOG(LogTemp, Warning, TEXT("EDIBLE ROW DATA: %d"), Data->Health)
}