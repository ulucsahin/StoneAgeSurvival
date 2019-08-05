// Fill out your copyright notice in the Description page of Project Settings.

#include "EquipmentManager.h"

// Sets default values for this component's properties
UEquipmentManager::UEquipmentManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UEquipmentManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEquipmentManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipmentManager::SetupManager(AHumanCharacter* Owner)
{
	this->Owner = Owner;
}

void UEquipmentManager::EquipItem(UEquipment* Equipment)
{
	// Equip Item
	Owner->EquipmentMapping[Equipment->EquipmentType]->SetSkeletalMesh(Equipment->SkeletalMesh);
	EquippedItems.Emplace(Equipment->EquipmentType, Equipment->ID);
		
}

void UEquipmentManager::UnequipItem(EEquipmentSlots ItemSlot)
{
	Owner->EquipmentMapping[ItemSlot]->SetSkeletalMesh(nullptr);
	EquippedItems.Remove(ItemSlot);
}
