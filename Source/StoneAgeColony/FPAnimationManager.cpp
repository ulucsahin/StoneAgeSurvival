// Fill out your copyright notice in the Description page of Project Settings.

#include "FPAnimationManager.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "StoneAgeColonyCharacter.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"

// Sets default values for this component's properties
UFPAnimationManager::UFPAnimationManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	static ConstructorHelpers::FObjectFinder<UAnimMontage> IdleBP(TEXT("'/Game/Uluc/FirstPerson/uluc_FPSArms/IdleAnim2/IdleHandsAnim2Montage.IdleHandsAnim2Montage'"));
	IdleAnimation = IdleBP.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> CutBP(TEXT("'/Game/Uluc/FirstPerson/uluc_FPSArms/KnifeCutAnim/KnifeCutAnimMontage.KnifeCutAnimMontage'"));
	CuttingAnimation = CutBP.Object;

	
}


// Called when the game starts
void UFPAnimationManager::BeginPlay()
{
	Super::BeginPlay();

	// ...

	
	PlayAnimation(EAnimations::VE_Idle);
	UE_LOG(LogTemp, Warning, TEXT("UFPAnimationManager::BeginPlay"));
}


void UFPAnimationManager::SetupManager(AStoneAgeColonyCharacter* Player, UWorld* World)
{
	this->Player = Player;
	this->World = World;
	this->AddToRoot();
	
}

// Called every frame
void UFPAnimationManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UFPAnimationManager::PlayAnimation(EAnimations Animation)
{
	UAnimInstance* AnimationInstance = Player->GetMesh1P()->GetAnimInstance();
	//auto AnimationLength = AnimInstance->Montage_Play(IdleAnimationEmptyHands, 1.f);
	if (AnimationInstance)
	{
		// If we want animation cancel this should be changed.
		if (CurrentAnimationFinished)
		{
			// We are starting new animation so it is not finished.
			CurrentAnimationFinished = false;

			// Stop currently looping animation
			World->GetTimerManager().ClearTimer(TimerHandle);
			float AnimationLength;
			if (Animation == EAnimations::VE_Idle)
			{
				AnimationLength = AnimationInstance->Montage_Play(IdleAnimation, 1.f);
			}
			else if (Animation == EAnimations::VE_Cutting)
			{
				AnimationLength = AnimationInstance->Montage_Play(CuttingAnimation, 1.f);
			}

			// Continue looping idle animation after current animation ended
			World->GetTimerManager().SetTimer(TimerHandle, this, &UFPAnimationManager::PlayIdleAnimation, AnimationLength - 0.3f, true);
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("plaksdfpaskdjfaslkjd"));
	}
		
}

void  UFPAnimationManager::PlayIdleAnimation()
{
	// Current animation finished so we loop idle animation
	CurrentAnimationFinished = true;
	UAnimInstance* AnimationInstance = Player->GetMesh1P()->GetAnimInstance();
	AnimationInstance->Montage_Play(IdleAnimation, 1.f);
}