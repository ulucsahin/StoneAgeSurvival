// Fill out your copyright notice in the Description page of Project Settings.

#include "MorningRegularAI.h"
#include "SettlementMember.h"
#include "Settlement.h"
#include "Farm.h"

AMorningRegularAI::AMorningRegularAI()
{

}

void AMorningRegularAI::Act()
{
	UE_LOG(LogTemp, Warning, TEXT("AMorningRegularAI::Act"));

	BreakfastTogether();
};

void AMorningRegularAI::BreakfastTogether()
{
	if (Possessed->BelongingSettlement)
	{
		for (auto Structure : Possessed->BelongingSettlement->Structures)
		{
			// Farm just for testing, later on it will be dining table or some other gathering point.
			auto Farm = Cast<AFarm>(Structure);
			if (Farm)
			{
				UE_LOG(LogTemp, Warning, TEXT("AMorningRegularAI::BreakfastTogether FARM FOUND"));
				MoveToLocation(Farm->GetActorLocation());
			}
		}
	}
	
}