// Fill out your copyright notice in the Description page of Project Settings.

#include "SettlementMemberAI.h"
#include "SettlementMember.h"
#include "Settlement.h"
#include "MyUtility.h"
#include "Communicator.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


ASettlementMemberAI::ASettlementMemberAI() 
{
	ShouldUpdate = false;
	
}

void ASettlementMemberAI::Possess(APawn *InPawn)
{
	Super::Possess(InPawn);
	ASettlementMember *Char = Cast<ASettlementMember>(InPawn);
	Possessed = Char;
	//Possessed->Activity = EActivity::VE_Idle;
	//Communicator::GetInstance().World->GetTimerManager().SetTimer(TimerHandle, this, &ASettlementMemberAI::Act, 1.0f, true);
}


void ASettlementMemberAI::Act()
{	
	
}


void ASettlementMemberAI::StopTimerHandle()
{
	Communicator::GetInstance().World->GetTimerManager().ClearTimer(TimerHandle);
}