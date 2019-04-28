// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "EnemyAI.h"
/* AI Include */
#include "Perception/PawnSensingComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* Our sensing component to detect players by visibility and noise checks. */
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->HearingThreshold = 600;
	PawnSensingComp->LOSHearingThreshold = 1200;
	FollowRadius = 1500.f;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{	
	Super::BeginPlay();
	GLog->Log("NPC started.");

	/* This is the earliest moment we can bind our delegates to the component */
	if (PawnSensingComp){
		GLog->Log("I sense some tingling.");
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnSeePlayer);
		PawnSensingComp->OnHearNoise.AddDynamic(this, &AEnemyCharacter::OnHearNoise);
	}
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//AEnemyAI* AIController = Cast<AEnemyAI>(GetController());
	//AIController->SetTargetInRange(NULL);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void AEnemyCharacter::OnSeePlayer(APawn* Pawn) {
	GLog->Log("I saw the sun");

	//bSensedTarget = true;

	AEnemyAI* AIController = Cast<AEnemyAI>(GetController());
	ACharacter* SensedPawn = Cast<ACharacter>(Pawn);

	// Allows AI to stop chasing target when target is out of range
	AIController->SetTargetInRange(true);

	if (AIController /*&& SensedPawn->IsAlive()*/ )
	{	
		GLog->Log("Controller is ok.");
		if (SensedPawn) {
			GLog->Log("SensedPawn");
		}

		AIController->SetTargetEnemy(SensedPawn);
	}

	FVector SensedPawnLocation = SensedPawn->GetActorLocation();
	FVector SelfLocation = GetActorLocation();
	float Distance = FVector::Dist(SelfLocation, SensedPawnLocation);
	UE_LOG(LogTemp, Warning, TEXT("Distance to player:, %f"), Distance);

	// Stop following once target is out of follow radius.
	// TODO: Line of sight.
	if (Distance > FollowRadius) {
		AIController->SetTargetInRange(false);
	}
}

void AEnemyCharacter::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume) {

}
