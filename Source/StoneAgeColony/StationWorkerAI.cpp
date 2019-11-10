// Fill out your copyright notice in the Description page of Project Settings.

#include "StationWorkerAI.h"
#include "Settlement.h"
#include "SettlementMember.h"
#include "CraftingStation.h"
//#include "MyUtility.h"
#include "Communicator.h"
#include "ObjectFactory.h"
#include "Templates/Tuple.h"

AStationWorkerAI::AStationWorkerAI()
{
	
}

void AStationWorkerAI::Possess(APawn* InstigatorPawn)
{
	Super::Possess(InstigatorPawn);
	//RemainingCraftList = Possessed->CraftList;
	Communicator::GetInstance().World->GetTimerManager().SetTimer(TimerHandle, this, &AStationWorkerAI::CheckStatus, 1.0f, true);
}

void AStationWorkerAI::Act()
{
	MoveToStation();

	// TEST
	//StartCrafting();
}

void AStationWorkerAI::CheckStatus()
{
	//Super::CheckStatus();
	//UE_LOG(LogTemp, Warning, TEXT("AStationWorkerAI::CheckStatus"));

	if (Possessed->Activity == EActivity::VE_GoingToStation || Possessed->Activity == EActivity::VE_Idle)
	{
		UE_LOG(LogTemp, Warning, TEXT("AStationWorkerAI::CheckStatus Distance to workstation: %f"), GetDistanceToWorkstation());
		if (GetDistanceToWorkstation() < 200.f)
		{
			StopMovement();
			Possessed->Activity = EActivity::VE_Working;
			StartCrafting();
		}

	}
	
}

void AStationWorkerAI::MoveToStation()
{
	//UE_LOG(LogTemp, Warning, TEXT("AStationWorkerAI::MoveToStation"));

	ASettlement* BelongingSettlement = nullptr;

	if(Possessed) BelongingSettlement = Possessed->BelongingSettlement;



	if (BelongingSettlement)
	{
		for (auto x : BelongingSettlement->Structures)
		{
			// if this structure is suitable for profession of member
			if (x->GetID() == Possessed->Profession.WorkstationTypeID)
			{
				// only select this structure if it belongs to current member or has no working member, aka if it is empty-available
				if (x->WorkingMember == nullptr || x->WorkingMember == (ASettlementMember*)GetPawn())
				{
					WorkStation = Cast<ACraftingStation>(x);
					x->WorkingMember = (ASettlementMember*)GetPawn();
					MoveToLocation(x->GetActorLocation());
					Possessed->Activity = EActivity::VE_GoingToStation;
					break;
				}
				
			}

		}

	}

}

int32 AStationWorkerAI::DecideItemToCraft()
{
	/*
	*
	NOT ANYMORE
	* Decides which item the Possessed NPC should craft next.
	* If no items are available and crafting orders are set to 0, this method returns -2.
	* If no items are available and crafting orders are NOT set to 0, this method should return -2 again.
	* This method should never return -1.
	NOT ANYMORE
	*
	*/

	int32 DecidedItem = -1;

	// If remaining craft list is empty reset it from CraftList of Possessed
	// First item of RemainingCraftList is the item we want to craft next
	if (Possessed->RemainingCraftList.Num() == 0)
	{
		for (auto Item : Possessed->CraftList)
		{
			for (int i = 0; i < Item.Value; i++)
			{
				Possessed->RemainingCraftList.Add(Item.Key);
			}
		}
	}

	// Possessed->CraftList may be empty aswell.
	if (Possessed->RemainingCraftList.Num() > 0)
	{
		DecidedItem = Possessed->RemainingCraftList[0];
		Possessed->RemainingCraftList.RemoveAt(0);
	}
	
	

	
	
	

	// Changing container during iteration gives errors. 
	// We need a seperate container to iterate.
	//TArray<TTuple<int32, int32>> CraftListAsArray = {};

	//for (auto Item : RemainingCraftList)
	//{
	//	CraftListAsArray.Add(MakeTuple(Item.Key, Item.Value));
	//}

	//for (int i = 0; i < CraftListAsArray.Num(); i++)
	//{
	//	auto ItemID = CraftListAsArray[i].Key;
	//	auto ItemAmount = CraftListAsArray[i].Value;
	//	if (ItemAmount <= 0)
	//	{
	//		CraftListAsArray[i] = MakeTuple(-1, -1);
	//		continue;
	//	}
	//	else
	//	{
	//		CraftListAsArray[i] = MakeTuple(ItemID, ItemAmount - 1);
	//		DecidedItem = ItemID;
	//	}

	//	break;
	//}

	//for (auto Item : CraftListAsArray)
	//{
	//	if (Item.Value > 0)
	//	{
	//		DecidedItem = -2;
	//		break;
	//	}
	//}

	//for (auto Item : RemainingCraftList)
	//{
	//	
	//	// Remove item entry from TMap if 0 of that item left
	//	if (Item.Value == 0)
	//	{
	//		RemainingCraftList.Remove(Item.Key);
	//		continue;
	//	}
	//	else
	//	{
	//		RemainingCraftList.Emplace(Item.Key, Item.Value - 1);
	//		DecidedItem = Item.Key;
	//	}

	//	break; 
	//}

	// if DecidedItem is -1 this means that we exhausted RemainingCraftList, reset the list from CraftList of possesssed and decide again.
	//if (DecidedItem == -1)
	//{
	//	RemainingCraftList = Possessed->CraftList;
	//	return DecideItemToCraft();
	//}

	return DecidedItem;
}

void AStationWorkerAI::StartCrafting()
{
	UE_LOG(LogTemp, Warning, TEXT("AStationWorkerAI::StartCraftingg T Trying Crafting"));
	if (WorkStation)
	{
		auto CraftableItems = WorkStation->CraftableItems;
		AObjectFactory* Factory = NewObject<AObjectFactory>();

		int32 DecidedItem = DecideItemToCraft();
		UE_LOG(LogTemp, Warning, TEXT("DecidedItem : %d"), DecidedItem);

		// If there are not enough items to create or crafting list are all set to 0.
		if (DecidedItem < 0 ) return; 


		// Currently creates first item on the Station's CraftableItems list. Need a way to select which one.
		auto ItemToCraft = Factory->CreateObjectBetter(DecidedItem);
		WorkStation->CurrentItem = ItemToCraft;
		WorkStation->CurrentItemID = ItemToCraft->GetID();
		WorkStation->SetCraftingCharacter(Possessed);
	
		if (WorkStation->CraftingRequirementsMet())
		{
			Possessed->Activity = EActivity::VE_Working;
			WorkStation->StartCrafting(5.f); // how to get time to craft?
		}
		
	}

}

float AStationWorkerAI::GetDistanceToWorkstation()
{
	float DistanceToWorkStation = 999999.f;

	if (WorkStation)
	{
		DistanceToWorkStation = FVector::Dist(Possessed->GetActorLocation(), WorkStation->GetActorLocation());
	}

	return DistanceToWorkStation;
}
