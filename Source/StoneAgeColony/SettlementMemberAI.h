// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SettlementMemberAI.generated.h"

class AStructure;
class ASettlementMember;

UENUM()
enum class EActivity : uint8
{
	VE_Idle, VE_Moving, VE_Working, VE_Talking,
};

UCLASS()
class STONEAGECOLONY_API ASettlementMemberAI : public AAIController
{
	GENERATED_BODY()
	
public:
	ASettlementMemberAI();

	virtual void Possess(APawn *InPawn) override;

	UFUNCTION()
	virtual void Act();

	void StopTimerHandle();

	FTimerHandle TimerHandle;
	uint8 EnemyKeyID;
	ASettlementMember* Possessed;
	bool ShouldUpdate;
	EActivity Activity;

};
