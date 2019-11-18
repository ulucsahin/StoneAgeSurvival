// Fill out your copyright notice in the Description page of Project Settings.

#include "SettlementMemberProfession.h"

// Sets default values for this component's properties
USettlementMemberProfession::USettlementMemberProfession()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USettlementMemberProfession::BeginPlay()
{
	Super::BeginPlay();
}

FProfession USettlementMemberProfession::GetProfession(EProfession Profession)
{
	if (Profession == EProfession::VE_Unoccupied)
	{
		return FUnoccupied{};
	}
	else if (Profession == EProfession::VE_Carpenter)
	{
		return FCarpenter{};
	}
	else if (Profession == EProfession::VE_StoneWorker)
	{
		return FStoneWorker{};
	}
	else if (Profession == EProfession::VE_Farmer)
	{
		return FFarmer{};
	}
	else
	{
		return FUnoccupied{};
	}
	
}
