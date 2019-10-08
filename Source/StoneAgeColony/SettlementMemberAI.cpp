// Fill out your copyright notice in the Description page of Project Settings.

#include "SettlementMemberAI.h"
//#include "EnemyCharacter.h"
#include "SettlementMember.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "SettlementMember.h"
#include "Settlement.h"

ASettlementMemberAI::ASettlementMemberAI() 
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
}

void ASettlementMemberAI::Possess(APawn *InPawn)
{
	Super::Possess(InPawn);

	ASettlementMember *Char = Cast<ASettlementMember>(InPawn);

	if (Char && Char->BotBehavior) 
	{
		BlackboardComp->InitializeBlackboard(*Char->BotBehavior->BlackboardAsset);
		EnemyKeyID = BlackboardComp->GetKeyID("Target");
		BehaviorComp->StartTree(*Char->BotBehavior);
	}
	
}

void ASettlementMemberAI::SetTargetEnemy(APawn* NewTarget) 
{
	if (BlackboardComp) 
	{
		BlackboardComp->SetValueAsObject("SensedActor", NewTarget);
	}
}

void ASettlementMemberAI::SetTargetInRange(bool InRange) 
{
	if (BlackboardComp) 
	{
		BlackboardComp->SetValueAsBool("TargetInRange", InRange);
	}
}

void ASettlementMemberAI::MoveToWorkingStation()
{
	ASettlementMember* Possessed = (ASettlementMember*)GetPawn();
	ASettlement* BelongingSettlement = Possessed->BelongingSettlement;

	if (BelongingSettlement)
	{
		for (auto x : BelongingSettlement->Structures)
		{
			if (x->GetID() == Possessed->Profession.WorkstationTypeID)
			{
				MoveToLocation(x->GetActorLocation());
			}

		}

	}
}