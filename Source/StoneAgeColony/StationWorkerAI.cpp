// Fill out your copyright notice in the Description page of Project Settings.

#include "StationWorkerAI.h"
#include "Settlement.h"
#include "SettlementMember.h"
//#include "MyUtility.h"
//#include "Communicator.h"

AStationWorkerAI::AStationWorkerAI()
{

}

void AStationWorkerAI::Act()
{
	MoveToStation();
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
					x->WorkingMember = (ASettlementMember*)GetPawn();
					MoveToLocation(x->GetActorLocation());
					Activity = EActivity::VE_Working;
					break;
				}
				
			}

		}

	}

}
