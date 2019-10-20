// Fill out your copyright notice in the Description page of Project Settings.

#include "NightRegularAI.h"
#include "SettlementMember.h"
#include "Settlement.h"
#include "House.h"

ANightRegularAI::ANightRegularAI()
{

}

void ANightRegularAI::Act()
{
	UE_LOG(LogTemp, Warning, TEXT("ANightRegularAI::Act"));
	GoHome();
}

void ANightRegularAI::GoHome()
{
	UE_LOG(LogTemp, Warning, TEXT("ANightRegularAI:: GoHome"));

	auto PossessedHomeID = Possessed->HomeSpecialID;

	for (auto Structure : Possessed->BelongingSettlement->Structures)
	{
		AHouse* House = Cast<AHouse>(Structure);
		if (House)
		{
			if (House->SpecialID == PossessedHomeID)
			{
				UE_LOG(LogTemp, Warning, TEXT("ANightRegularAI::GoHome HOUSE FOUND"));
				MoveToLocation(House->GetActorLocation());
				return;
			}

		}

	}
}