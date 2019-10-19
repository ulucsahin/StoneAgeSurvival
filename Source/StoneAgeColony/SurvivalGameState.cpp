// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalGameState.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "SettlementMember.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ASurvivalGameState::ASurvivalGameState(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	/* 1 SECOND real time is 1*TimeScale MINUTES game time */
	TimeScale = 8.0f;
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

void ASurvivalGameState::UpdateNPCAI()
{
	UE_LOG(LogTemp, Warning, TEXT("ASurvivalGameState::UpdateNPCAI"));
	
	static bool AlreadyMorningAI = false;
	static bool AlreadyRealAI = false;
	static bool AlreadyNightAI = false;

	TArray<AActor*> FoundSettlementMembers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASettlementMember::StaticClass(), FoundSettlementMembers);

	if ((GetTimeOfDay() < 60.f * 8.f) && (GetTimeOfDay() > 60.f * 6.f) && !AlreadyMorningAI) // between 6 am - 8 am
	{
		UE_LOG(LogTemp, Warning, TEXT("ASurvivalGameState::UpdateNPCAI SET MORNING AI"));
		AlreadyMorningAI = true;
		AlreadyRealAI = false;
		AlreadyNightAI = false;

		for (auto Member : FoundSettlementMembers)
		{
			((ASettlementMember*)Member)->SetupAIController();
			((ASettlementMember*)Member)->GetNotification();
		}
	}
	else if ((GetTimeOfDay() > 60.f * 8.f) && (GetTimeOfDay() < 60.f * 20.f) && !AlreadyRealAI) // between 8am-8pm
	{
		UE_LOG(LogTemp, Warning, TEXT("ASurvivalGameState::UpdateNPCAI  SET REAL AI "));
		AlreadyMorningAI = false;
		AlreadyRealAI = true;
		AlreadyNightAI = false;

		for (auto Member : FoundSettlementMembers)
		{
			((ASettlementMember*)Member)->SetupAIController();
			((ASettlementMember*)Member)->GetNotification();
		}
	}
	else if (((GetTimeOfDay() < 60.f * 6.f) || (GetTimeOfDay() > 60.f * 22.f))  && !AlreadyNightAI) // between 10pm - 6am
	{
		UE_LOG(LogTemp, Warning, TEXT("ASurvivalGameState::UpdateNPCAI SET NIGHT AI "));
		AlreadyMorningAI = false;
		AlreadyRealAI = false;
		AlreadyNightAI = true;

		for (auto Member : FoundSettlementMembers)
		{
			((ASettlementMember*)Member)->SetupAIController();
			((ASettlementMember*)Member)->GetNotification();
		}
	}
	

}

float ASurvivalGameState::GetTimeOfDayIncrement() {
	return (GetWorldSettings()->GetEffectiveTimeDilation() * TimeScale);
}

void ASurvivalGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASurvivalGameState, ElapsedGameMinutes);
	DOREPLIFETIME(ASurvivalGameState, bIsNight);
}

float ASurvivalGameState::GetTimeOfDay() 
{
	return ElapsedGameMinutes % (24*60);
}
