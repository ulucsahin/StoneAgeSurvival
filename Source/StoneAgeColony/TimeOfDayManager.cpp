// Fill out your copyright notice in the Description page of Project Settings.

#include "TimeOfDayManager.h"
#include "StoneAgeColony.h"
#include "SurvivalGameState.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/SkyLightComponent.h"


// Sets default values
ATimeOfDayManager::ATimeOfDayManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	AmbientAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AmbientAudioComp"));
	AmbientAudioComp->bAutoActivate = false;

	/* Treshold to recapture the skylight */
	RequiredCaptureDelta = 0.01f;

	SetReplicates(true);

}

// Called when the game starts or when spawned
void ATimeOfDayManager::BeginPlay()
{
	Super::BeginPlay();
	GLog->Log("ATimeOfDayManager::BeginPlay");

	if (PrimarySunLight)
	{
		GLog->Log("ATimeOfDayManager::BeginPlay PrimarySunLight");
		OriginalSunBrightness = PrimarySunLight->GetBrightness();
		TargetSunBrightness = OriginalSunBrightness;
	}

	// NOT YET IMPLEMENTED
	PlayAmbientLoop();
}

// Called every frame
void ATimeOfDayManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ASurvivalGameState* MyGameState = Cast<ASurvivalGameState>(GetWorld()->GetGameState());

	if (MyGameState)
	{
		/* Update the position of the sun. */
		if (PrimarySunLight)
		{	

			if (LastTimeOfDay == MyGameState->ElapsedGameMinutes)
			{
				TimeSinceLastIncrement += DeltaTime;
			}
			else
			{
				/* Reset prediction */
				TimeSinceLastIncrement = 0;
			}

			/* Predict the movement of the sun to smooth out the rotations between replication updates of the actual time of day */
			const float PredictedIncrement = MyGameState->GetTimeOfDayIncrement() * TimeSinceLastIncrement;

			/* TimeOfDay is expressed in minutes, we need to convert this into a pitch rotation */
			const float MinutesInDay = 24 * 60;
			const float PitchOffset = 90; /* The offset to account for time of day 0 should equal midnight */
			const float PitchRotation = 360 * ((MyGameState->ElapsedGameMinutes + PredictedIncrement) / MinutesInDay);

			FRotator NewSunRotation = FRotator(PitchRotation + PitchOffset, 45.0f, 0);
			PrimarySunLight->SetActorRelativeRotation(NewSunRotation);

			LastTimeOfDay = MyGameState->ElapsedGameMinutes;
		}
		 
		bool CurrentNightState = MyGameState->GetIsNight();

		//UE_LOG(LogTemp, Warning, TEXT("CurrentNightState :, %s, LastNightState :, %s"), (CurrentNightState ? TEXT("True") : TEXT("False")), (LastNightState ? TEXT("True") : TEXT("False")));
		if (CurrentNightState != LastNightState)
		{
			if (CurrentNightState)
			{
				// Play night started cue (position is irrelevant for non spatialized & attenuated sounds)
				UGameplayStatics::PlaySoundAtLocation(this, SoundNightStarted, GetActorLocation());
				TargetSunBrightness = 0.01f;
			}
			else
			{
				// Play daytime started cue (position is irrelevant for non spatialized & attenuated sounds)
				UGameplayStatics::PlaySoundAtLocation(this, SoundNightEnded, GetActorLocation());
				TargetSunBrightness = OriginalSunBrightness;
			}

			/* Change to a new ambient loop */
			PlayAmbientLoop();
		}

		/* Update sun brightness to transition between day and night
			(Note: We cannot disable the sunlight because BP_SkySphere depends on an enabled light to update the skydome) */
		const float LerpSpeed = 0.1f * GetWorldSettings()->GetEffectiveTimeDilation();
		float CurrentSunBrightness = PrimarySunLight->GetBrightness();
		float NewSunBrightness = FMath::Lerp(CurrentSunBrightness, TargetSunBrightness, LerpSpeed);
		PrimarySunLight->SetBrightness(NewSunBrightness);

		LastNightState = CurrentNightState;
	}

	UpdateSkylight();

}


void ATimeOfDayManager::UpdateSkylight() {
	if (SkyLightActor)
	{
		ASurvivalGameState* MyGameState = Cast<ASurvivalGameState>(GetWorld()->GetGameState());
		if (MyGameState)
		{
			const float MinutesInDay = 24 * 60;
			/* Map the intensity from 0 - 12 - 24 hours into 0 - 1 - 0 alpha */
			const float Alpha = FMath::Sin((MyGameState->GetElapsedMinutesCurrentDay() / MinutesInDay) * 3.14);

			/* Update Intensity */
			const float NewIntensity = FMath::Lerp(0.1, 1.0, Alpha);
			SkyLightActor->GetLightComponent()->SetIntensity(NewIntensity);

			//float ColorChannel = 1.0f * NewIntensity;
			//SkyLightActor->GetLightComponent()->SetLightColor(FLinearColor(ColorChannel, ColorChannel, ColorChannel, 1.0f));

			if (RequiredCaptureDelta < FMath::Abs(NewIntensity - LastCapturedIntensity))
			{
				GLog->Log("RequiredCaptureDelta < FMath::Abs(NewIntensity - LastCapturedIntensity)");
				/* Force re-capture of the sky w/ new intensity */
				/* Hacky and costly solution to recapturing the sky, official support NYI */
				SkyLightActor->GetLightComponent()->RecaptureSky();
				LastCapturedIntensity = NewIntensity;
			}
		}
	}
}

void ATimeOfDayManager::PlayAmbientLoop() {
	/*AmbientAudioComp->Stop();

	ASurvivalGameState* MyGameState = Cast<ASurvivalGameState>(GetWorld()->GetGameState());
	if (MyGameState)
	{
		if (MyGameState->GetIsNight())
		{
			AmbientAudioComp->SetSound(AmbientNight);
		}
		else
		{
			AmbientAudioComp->SetSound(AmbientDaytime);
		}
	}

	AmbientAudioComp->Play();*/
}
