// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Runtime/Engine/Classes/Components/AudioComponent.h"
//#include "Runtime/Engine/Classes/Engine/DirectionalLight.h"
//#include "Runtime/Engine/Classes/Sound/SoundCue.h"
//#include "Runtime/Engine/Classes/Engine/SkyLight.h"
#include "TimeOfDayManager.generated.h"

class UAudioComponent;
class ADirectionalLight;
class USoundCue;
class ASkyLight;


UCLASS()
class STONEAGECOLONY_API ATimeOfDayManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimeOfDayManager();

	/* Cached bool of bIsNight to determine when we entered/left the night */
	bool LastNightState;

	/* Sunbrightness as specified in the level (considered as daytime sun) */
	float OriginalSunBrightness;

	/* Target brightness to lerp towards */
	float TargetSunBrightness;

	/* Required difference in Sky intensity before we re-capture the sky */
	float RequiredCaptureDelta;

	/* Last skylight intensity that was captured */
	float LastCapturedIntensity;

	/* Last local time of day to apply sun movement prediction */
	float LastTimeOfDay;

	/* Delta seconds since the time of day was last updated */
	float TimeSinceLastIncrement;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UAudioComponent* AmbientAudioComp;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundNightStarted;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundNightEnded;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* AmbientDaytime;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* AmbientNight;

	/* Primary sun of the level. Assigned in Blueprint during BeginPlay (BlueprintReadWrite is required as tag instead of EditDefaultsOnly) */
	UPROPERTY(BlueprintReadWrite, Category = "DayNight")
	ADirectionalLight* PrimarySunLight;

	UPROPERTY(BlueprintReadWrite, Category = "DayNight")
	ASkyLight* SkyLightActor;

	// UPROPERTY(BlueprintReadWrite, Category = "SkySphereClass")
	// TSubclassOf<AActor> BP_SkySphere;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Play the ambient loop for the current time state */
	void PlayAmbientLoop();
	
	void UpdateSkylight();



};
