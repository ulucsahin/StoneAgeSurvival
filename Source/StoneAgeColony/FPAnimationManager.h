// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FPAnimationManager.generated.h"

class AStoneAgeColonyCharacter;

enum EAnimations : uint8 {
	VE_Idle  UMETA(DisplayName = "Idle"),
	VE_Cutting  UMETA(DisplayName = "Cutting")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STONEAGECOLONY_API UFPAnimationManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFPAnimationManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PlayAnimation(EAnimations Animation);

	AStoneAgeColonyCharacter* Player;
	UWorld* World;

private:
	class UAnimMontage* CurrentAnimation;
	class UAnimMontage* IdleAnimation;
	class UAnimMontage* CuttingAnimation;
	FTimerHandle TimerHandle; // timer for looping idle animation

	bool CurrentAnimationFinished = true;

	UFUNCTION() // ufunction needed for timer
	void PlayIdleAnimation();
};

