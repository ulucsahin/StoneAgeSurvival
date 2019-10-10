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
	ASettlement* BelongingSettlement = nullptr;
	if(Possessed)
		BelongingSettlement = Possessed->BelongingSettlement;

	if (BelongingSettlement)
	{
		UE_LOG(LogTemp, Warning, TEXT("if BelongingSettlement"));
		for (auto x : BelongingSettlement->Structures)
		{
			UE_LOG(LogTemp, Warning, TEXT("BelongingSettlement->Structures"));
			// if this structure is suitable for profession of member
			if (x->GetID() == Possessed->Profession.WorkstationTypeID)
			{
				// only select this structure if it belongs to current member or has no working member, aka if it is empty-available
				if (x->WorkingMember == nullptr || x->WorkingMember == (ASettlementMember*)GetPawn())
				{
					x->WorkingMember = (ASettlementMember*)GetPawn();
					MoveToLocation(x->GetActorLocation());
					break;
				}
				
			}

		}

	}
}

void ASettlementMemberAI::WanderAround()
{
	
}