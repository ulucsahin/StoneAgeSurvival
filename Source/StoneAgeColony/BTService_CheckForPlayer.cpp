// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckForPlayer.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "StoneAgeColonyCharacter.h"
#include "EnemyAI.h"
#include "EnemyCharacter.h"
#include <string>


UBTService_CheckForPlayer::UBTService_CheckForPlayer() 
{
	bCreateNodeInstance = true;
}

void UBTService_CheckForPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
	AEnemyAI *EnemyPC = Cast<AEnemyAI>(OwnerComp.GetAIOwner());
	UE_LOG(LogTemp, Warning, TEXT("UBTService_CheckForPlayer::TickNode Enemy"));
	if (EnemyPC) 
	{
		APawn *Enemy = GetWorld()->GetFirstPlayerController()->GetPawn();

		if (Enemy) 
		{
			
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(EnemyPC->EnemyKeyID, Enemy);
			
		}
	}
}

