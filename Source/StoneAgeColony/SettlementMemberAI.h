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
	VE_Idle, // waiting with no purpose
	VE_Moving, // moving around aimlessly
	VE_Working, // currently crafting
	VE_Talking, // Talking with player (or other npcs later on?)
	VE_GoingToStation, // moving to its working station
	VE_FinishedJob, // finished crafting, ready for crafting more or going home
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

	virtual void CheckStatus();

	void StopTimerHandle();

	FTimerHandle TimerHandle;
	uint8 EnemyKeyID;
	ASettlementMember* Possessed;
	bool ShouldUpdate;
	//EActivity Activity;

};
