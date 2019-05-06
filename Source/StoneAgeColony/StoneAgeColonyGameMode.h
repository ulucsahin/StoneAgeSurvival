// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StoneAgeColonyGameMode.generated.h"

UCLASS(minimalapi)
class AStoneAgeColonyGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HudWidget", Meta = (BlueprintProtected = "True"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY()
	UUserWidget* WidgetInstance;

public:
	AStoneAgeColonyGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void InitGameState();
	virtual void PreInitializeComponents() override;
	virtual void DefaultTimer();
	virtual void StartMatch();
	virtual void OnNightEnded();


	/*Player start override*/
	// https://youtu.be/2aUYBzmefpM?t=355
	/*virtual class AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	bool ShouldSpawnAtStartSpot(AController* Player) override;*/


	/* Can we deal damage to players in the same team */
	UPROPERTY(EditDefaultsOnly, Category = "Rules")
	bool bAllowFriendlyFireDamage;

	/* Allow zombie spawns to be disabled (for debugging) */
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bSpawnZombiesAtNight;

	float BotSpawnInterval;

	/* The start time for the gamemode */
	int32 TimeOfDayStart;

	/* The teamnumber assigned to Players */
	int32 PlayerTeamNum;

	/* Handle for efficient management of DefaultTimer timer */
	FTimerHandle TimerHandle_DefaultTimer;

	/* Keep reference to the night state of the previous frame */
	bool LastIsNight;

};



