// Fill out your copyright notice in the Description page of Project Settings.

#include "UnoccupiedAI.h"
#include "SettlementMember.h"
#include "Settlement.h"
#include "MyUtility.h"
#include "Communicator.h"

AUnoccupiedAI::AUnoccupiedAI()
{

}

void AUnoccupiedAI::Possess(APawn *InPawn)
{
	Super::Possess(InPawn);

	//UE_LOG(LogTemp, Warning, TEXT("AUnoccupiedAI::Possess"));

	//Communicator::GetInstance().World->GetTimerManager().SetTimer(TimerHandle, this, &AUnoccupiedAI::CheckStatus, 1.0f, true);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AUnoccupiedAI::CheckStatus, 1.0f, true);
}

void AUnoccupiedAI::Act()
{	
	//WanderAround();
	//UE_LOG(LogTemp, Warning, TEXT("AUnoccupiedAI::Act"));
	

	if (Possessed->Activity == EActivity::VE_Idle)
	{
		// with 5% chance move around
		auto RandomValue = MyUtility::RandomFloat(0.f, 1.f);
		//UE_LOG(LogTemp, Warning, TEXT("AUnoccupiedAI::Act RandomValue: %f"), RandomValue);
		if (RandomValue > 0.50f)
		{
			WanderAround();
		}

	}

}

void AUnoccupiedAI::CheckStatus()
{
	Super::CheckStatus();
	//UE_LOG(LogTemp, Warning, TEXT("AUnoccupiedAI::CheckStatus"));
	if (Possessed->Activity == EActivity::VE_Idle)
	{
		auto RandomValue = MyUtility::RandomFloat(0.f, 1.f);
		//UE_LOG(LogTemp, Warning, TEXT("AUnoccupiedAI::CheckStatus RandomValue: %f"), RandomValue);
		if (RandomValue > 0.50f)
		{
			WanderAround();
		}

	}

}

void AUnoccupiedAI::WanderAround()
{
	if (Possessed->BelongingSettlement)
	{
		Possessed->Activity = EActivity::VE_Moving;
		
		auto SettlementLocation = Possessed->BelongingSettlement->GetActorLocation();
		auto Radius = Possessed->BelongingSettlement->AreaRadius;
		
		FVector TargetLocation = FVector(MyUtility::RandomFloat(-Radius, Radius), MyUtility::RandomFloat(-Radius, Radius), 0.f);
		MoveToLocation(TargetLocation + SettlementLocation);
		
		//UE_LOG(LogTemp, Warning, TEXT("AUnoccupiedAI::WanderAround TargetLocation %f, %f, %f"), TargetLocation.X, TargetLocation.Y, TargetLocation.Z);
		//UE_LOG(LogTemp, Warning, TEXT("AUnoccupiedAI::WanderAround SettlementLocation %f, %f, %f"), SettlementLocation.X, SettlementLocation.Y, SettlementLocation.Z);
	}
}