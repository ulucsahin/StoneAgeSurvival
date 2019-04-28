// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalGameState.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Net/UnrealNetwork.h"

ASurvivalGameState::ASurvivalGameState(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	/* 1 SECOND real time is 1*TimeScale MINUTES game time */
	TimeScale = 80.0f;
	bIsNight = false;

	SunriseTimeMark = 6.0f;
	SunsetTimeMark = 18.0f;
}

bool ASurvivalGameState::GetIsNight()
{
	return bIsNight;
}

int32 ASurvivalGameState::GetElapsedMinutesCurrentDay() {
	return ElapsedGameMinutes - GetElapsedFullDaysInMinutes();
}

int32 ASurvivalGameState::GetElapsedDays()
{
	const float MinutesInDay = 24 * 60;
	const float ElapsedDays = ElapsedGameMinutes / MinutesInDay;
	return FMath::FloorToInt(ElapsedDays);
}

int32 ASurvivalGameState::GetElapsedFullDaysInMinutes()
{
	const int32 MinutesInDay = 24 * 60;
	return GetElapsedDays() * MinutesInDay;
}

bool ASurvivalGameState::GetAndUpdateIsNight()
{
	const float TimeOfDay = ElapsedGameMinutes - GetElapsedFullDaysInMinutes();
	if (TimeOfDay > (SunriseTimeMark * 60) && TimeOfDay < (SunsetTimeMark * 60))
	{
		bIsNight = false;
	}
	else
	{
		bIsNight = true;
	}

	return bIsNight;
}

float ASurvivalGameState::GetTimeOfDayIncrement() {
	return (GetWorldSettings()->GetEffectiveTimeDilation() * TimeScale);
}

void ASurvivalGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASurvivalGameState, ElapsedGameMinutes);
	DOREPLIFETIME(ASurvivalGameState, bIsNight);
	DOREPLIFETIME(ASurvivalGameState, TotalScore);
}