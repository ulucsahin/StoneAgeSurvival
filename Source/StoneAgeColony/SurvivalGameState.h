// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SurvivalGameState.generated.h"

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API ASurvivalGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASurvivalGameState(const class FObjectInitializer& ObjectInitializer);

	/* Returns the time of day increment every real second (converted to accelerated game time, eg. 1 real second is 1 minute in time of day increment) */
	float GetTimeOfDayIncrement();

	/* Total accumulated score from all players  */
	UPROPERTY(Replicated)
	int32 TotalScore;

	/* Current time of day in the gamemode represented in full minutes */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "TimeOfDay")
	int32 ElapsedGameMinutes;

	UPROPERTY(Replicated)
	bool bIsNight;

	/* Time in wallclock hours the day begins */
	float SunriseTimeMark;

	/* Time in wallclock hours the night begins */
	float SunsetTimeMark;

	/* Conversion of 1 second real time to X seconds gametime of the day/night cycle */
	UPROPERTY(EditDefaultsOnly, Category = "TimeOfDay")
	float TimeScale;

	bool GetIsNight();

	bool GetAndUpdateIsNight();

	int32 GetElapsedMinutesCurrentDay();

	UFUNCTION(BlueprintCallable, Category = "TimeOfDay")
	int32 GetElapsedDays();

	/* Returns whole days elapsed, represented in minutes */
	UFUNCTION(BlueprintCallable, Category = "TimeOfDay")
	int32 GetElapsedFullDaysInMinutes();

	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetTimeOfDay();
};
