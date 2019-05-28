// Fill out your copyright notice in the Description page of Project Settings.

#include "GatherableTree.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Communicator.h"
#include "StoneAgeColonyCharacter.h"

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
		((AStoneAgeColonyCharacter*)InstigatorPawn)->AddToInventory(this->ID);
	}


}

int AGatherableTree::GetID()
{
	return ID;
}
