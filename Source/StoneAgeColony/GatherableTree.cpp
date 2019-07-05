// Fill out your copyright notice in the Description page of Project Settings.

#include "GatherableTree.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Communicator.h"
#include "StoneAgeColonyCharacter.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

// Static Variables
//TSubclassOf<AGatherableTree> AGatherableTree::GatherableTreeBlueprint;

AGatherableTree::AGatherableTree(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// im not sure if this calls super's constructor
	static ConstructorHelpers::FObjectFinder<UTexture2D> InventoryTexObj(TEXT("Texture2D'/Game/Uluc/HUD/ItemIcons/TreeIcon.TreeIcon'"));
	InventoryTexture = InventoryTexObj.Object;

}

void AGatherableTree::BeginPlay()
{
	WoodAmount = 100;
	LastGatherTime = -100.f;

}

void AGatherableTree::OnUsed(APawn* InstigatorPawn)
{

	float GatherTime = Communicator::GetInstance().World->GetTimeSeconds();
	
	// Put harvesting on a cooldown
	// Can this cause int overflow after certain time?
	if (GatherTime - LastGatherTime > 2.f)
	{
		WoodAmount -= 10;
		UE_LOG(LogTemp, Warning, TEXT("Tree Harvested at %f , remaining wood: %d"), GatherTime, WoodAmount);
		LastGatherTime = GatherTime;
		int AmountToAdd = 10;
		((AStoneAgeColonyCharacter*)InstigatorPawn)->AddToInventory(this->ID, AmountToAdd);
	}


}

int AGatherableTree::GetID()
{
	return ID;
}

void AGatherableTree::RegisterActorDetailsToSave() {
	FGatherableTreeDetails TreeDetails;

	// Assign details to struct.
	TreeDetails.Transform = GetActorTransform();

	// Save details as struct to communicator. Which will be used during saving.
	Communicator::GetInstance().SpawnedGatherableTreeDetails.Add(TreeDetails);
}

void AGatherableTree::EmptyCommunicatorDetailsArray()
{
	Communicator::GetInstance().SpawnedGatherableTreeDetails.Empty();
}

//TArray<FGatherableTreeDetails> AGatherableTree::GetCommunicatorDetailsArray()
//{
//	return Communicator::GetInstance().SpawnedGatherableTreeDetails;
//}
//
//TSubclassOf<AGatherableTree> AGatherableTree::GetActorToSpawn()
//{
//	return Communicator::GetInstance().GatherableTreeToSpawn;
//}

void AGatherableTree::SpawnLoadedActors()
{
	/* Spawn previously saved characters from savefile. */

	FActorSpawnParameters SpawnParams;

	// Get actor details to spawn from communicator.
	auto ActorToSpawn = Communicator::GetInstance().GatherableTreeBlueprint;

	// Iterate over array and saved spawn actors.
	for (auto Details : Communicator::GetInstance().SpawnedGatherableTreeDetails)
	{
		FTransform ActorTransform = Details.Transform;
		Communicator::GetInstance().World->SpawnActor<AGatherableTree>(ActorToSpawn, ActorTransform, SpawnParams);
	}
}