// Fill out your copyright notice in the Description page of Project Settings.

#include "StationWorkerAI.h"
#include "Settlement.h"
#include "SettlementMember.h"
#include "CraftingStation.h"
//#include "MyUtility.h"
#include "Communicator.h"
#include "ObjectFactory.h"

AStationWorkerAI::AStationWorkerAI()
{
	
}

void AStationWorkerAI::Possess(APawn* InstigatorPawn)
{
	Super::Possess(InstigatorPawn);
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

	if(Possessed)
		BelongingSettlement = Possessed->BelongingSettlement;

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

void AStationWorkerAI::StartCrafting()
{
	UE_LOG(LogTemp, Warning, TEXT("AStationWorkerAI::StartCrafting Trying Crafting"));
	if (WorkStation)
	{
		auto CraftableItems = WorkStation->CraftableItems;
		AObjectFactory* Factory = NewObject<AObjectFactory>();

		// Currently creates first item on the Station's CraftableItems list. Need a way to select which one.
		auto ItemToCraft = Factory->CreateObjectBetter(CraftableItems[1]);
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
