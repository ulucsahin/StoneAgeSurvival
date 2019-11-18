// Fill out your copyright notice in the Description page of Project Settings.

#include "FarmerAI.h"
#include "Communicator.h"
#include "Farm.h"
#include "Plant.h"

AFarmerAI::AFarmerAI()
{
	MinDistanceToStation = 500.f;
}

void AFarmerAI::Act()
{
	Super::Act();
}

void AFarmerAI::Possess(APawn* InstigatorPawn)
{
	Super::Possess(InstigatorPawn);
}

void AFarmerAI::CheckStatus()
{	
	//Super::CheckStatus();
	UE_LOG(LogTemp, Warning, TEXT("AFarmerAI::CheckStatus"));

	if (Possessed->Activity == EActivity::VE_GoingToStation || Possessed->Activity == EActivity::VE_Idle)
	{
		if (GetDistanceToWorkstation() < MinDistanceToStation)
		{
			StopMovement();
			WorkStation->SetCraftingCharacter(Possessed);
			Possessed->Activity = EActivity::VE_Working;
			auto Plant = StartPlanting();
			// CONTINUE HERE-- HARVESTING PLANTS
			// CONTINUE HERE-- HARVESTING PLANTS
			// CONTINUE HERE-- HARVESTING PLANTS

		}

	}
}

APlant* AFarmerAI::StartPlanting()
{
	APlant* Plant = nullptr;
	auto Farm = Cast<AFarm>(WorkStation);
	
	if (Farm)
	{
		auto Socket = SelectSocketToPlant();
		if (Socket.IsEqual(""))
		{
			return nullptr;
		}


		Plant = Farm->Plant(700, Socket, false);

		Possessed->Activity = EActivity::VE_Working;
	}
	return Plant;
}

FName AFarmerAI::SelectSocketToPlant()
{
	auto Farm = Cast<AFarm>(WorkStation);

	FString SocketName = "";
	if (Farm)
	{
		for (auto Item : Farm->SocketFull)
		{	
			bool IsFull = Item.Value;
			if (!IsFull)
			{
				SocketName = Item.Key;
			}

		}
	}

	return FName(*SocketName);
}

void AFarmerAI::HarvestGrownPlants()
{
	auto Farm = Cast<AFarm>(WorkStation);
	auto PlantsGrown = Farm->GetPlantProgress();
	int32 ayaya = 15;
	// CONTINUE HERE-- HARVESTING PLANTS
	// CONTINUE HERE-- HARVESTING PLANTS
	// CONTINUE HERE-- HARVESTING PLANTS
	// CONTINUE HERE-- HARVESTING PLANTS
	// CONTINUE HERE-- HARVESTING PLANTS

}
