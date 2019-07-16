// Fill out your copyright notice in the Description page of Project Settings.

#include "UIInventoryItem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "StoneAgeColonyCharacter.h"
#include "UsableActor.h"
#include "Communicator.h"

UUIInventoryItem::UUIInventoryItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UUIInventoryItem::OnClick()
{
	UE_LOG(LogTemp, Warning, TEXT("Inventory Item Clicked, ID: %d"), ItemID);
	
}

void UUIInventoryItem::SetupInventoryItemCell()
{
	Super::SetupInventoryItemCell();

}