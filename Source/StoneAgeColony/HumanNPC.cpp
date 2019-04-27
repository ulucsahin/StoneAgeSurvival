// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanNPC.h"
#include "Classes/Components/StaticMeshComponent.h"

// Sets default values
AHumanNPC::AHumanNPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (GetWorld() && !GetWorld()->bStartup && GetController() == nullptr){
		SpawnDefaultController();
	}

	AIControllerClass = AHumanAIController::StaticClass();
}

// Called when the game starts or when spawned
void AHumanNPC::BeginPlay(){
	Super::BeginPlay();

	ai = Cast<AHumanAIController>(GetController());
	// FVector(132970.0f, 67990.0f, -8110.0f);

	FVector TargetLocation = FVector(xLoc, yLoc, zLoc);
	ai->MoveToLocation(TargetLocation, 10.0f, true, true, true);
	
	
	// Check movement every x seconds and act accordingly
	float UpdateFrequency = 1.0f;
	float StartAfter = 2.0f;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AHumanNPC::Act, UpdateFrequency, true, StartAfter);
}

// Called every frame
void AHumanNPC::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;
}

// Called to bind functionality to input
void AHumanNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHumanNPC::MoveForward(float Speed) {
	ForwardVector = GetActorForwardVector();
	AddMovementInput(FVector(Speed * ForwardVector.X, Speed * ForwardVector.Y, Speed * ForwardVector.Z));
}


void AHumanNPC::Rotate(float Degrees) {

}

void AHumanNPC::Act() {
	//0: Idle, 1:Waiting, 2:Paused, 3:Moving
	if (ai->GetMoveStatus() == 0) {
		FVector CurrentLoc = GetActorLocation();
		//GLog->Log("Idle");
		//GetActorRotation
		FRotator rotation = GetActorRotation();
		ai->MoveToLocation(FVector(132970.0f, 67990.0f, -8110.0f), 10.0f, true, true, true);
		//ai->MoveToLoc(this, FVector(132970.0f, 67990.0f, -8110.0f), 10.0f, true, true, true);
	}
	else if (ai->GetMoveStatus() == 1) {
		FVector CurrentLoc = GetActorLocation(); // dummy code
		// GLog->Log("Waiting");
	}
	else if (ai->GetMoveStatus() == 2) {
		FVector CurrentLoc = GetActorLocation(); // dummy code
		// GLog->Log("Paused");
	}
	else if (ai->GetMoveStatus() == 3) {
		FVector CurrentLoc = GetActorLocation(); // dummy code
		// GLog->Log("Moving");
	}
}



//AddMovementInput(FVector(1.f, 1.f, 1.f), 1.f);
//PlayAnimMontage(Animation);
// Rotate
//FRotator Rotation = FRotator(PitchValue, YawValue, RollValue);
//Rotation.Yaw += 0.3f;
//FQuat QuatRotation = FQuat(Rotation);
//AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
// Move 
//MoveForward(0.5f);
//PlayAnimMontage
//GetMesh()->PlayAnimation(GetMesh()->GetAnimInstance(), true);
//GetMesh()->GetAnimInstance();
