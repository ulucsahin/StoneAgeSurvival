// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "EnemyAI.h"
#include "Communicator.h"
#include "UObject/ConstructorHelpers.h"
/* AI Include */
#include "Perception/PawnSensingComponent.h"

// Static variables
//TSubclassOf<AEnemyCharacter> AEnemyCharacter::EnemyCharacterBlueprint;

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	/* Our sensing component to detect players by visibility and noise checks. */
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->HearingThreshold = 600;
	PawnSensingComp->LOSHearingThreshold = 1200;
	FollowRadius = 1500.f;

	UE_LOG(LogTemp, Warning, TEXT("AEnemyCharacter::AEnemyCharacter"));
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{	
	Super::BeginPlay();

	/* This is the earliest moment we can bind our delegates to the component */
	if (PawnSensingComp){
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnSeePlayer);
		PawnSensingComp->OnHearNoise.AddDynamic(this, &AEnemyCharacter::OnHearNoise);
	}

	// Set MorphManager 
	MorphManager = NewObject<UMorphManager>();
	MorphManager->SetupManager(this);

	UE_LOG(LogTemp, Warning, TEXT("AEnemyCharacter::BeginPlay"));
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacter::OnSeePlayer(APawn* Pawn) {
	AEnemyAI* AIController = Cast<AEnemyAI>(GetController());
	ACharacter* SensedPawn = Cast<ACharacter>(Pawn);

	// Allows AI to stop chasing target when target is out of range
	AIController->SetTargetInRange(true);

	if (AIController /*&& SensedPawn->IsAlive()*/ )
	{	
		if (SensedPawn) {
			AIController->SetTargetEnemy(SensedPawn);
		}
	}

	FVector SensedPawnLocation = SensedPawn->GetActorLocation();
	FVector SelfLocation = GetActorLocation();
	float Distance = FVector::Dist(SelfLocation, SensedPawnLocation);

	// Stop following once target is out of follow radius.
	// TODO: Line of sight.
	if (Distance > FollowRadius) {
		AIController->SetTargetInRange(false);
	}
}

void AEnemyCharacter::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume) 
{

}

void AEnemyCharacter::RegisterActorDetailsToSave() 
{	
	// Assign details to struct.
	CharDetails.Transform = GetActorTransform();
	CharDetails.FaceDetails = MorphManager->FaceDetails;

	// Save details as struct to communicator. Which will be used during saving.
	Communicator::GetInstance().SpawnedCharacterDetails.Add(CharDetails);

	UE_LOG(LogTemp, Warning, TEXT("EnemyCharacter added to communicator."));
}

void AEnemyCharacter::EmptyCommunicatorDetailsArray()
{
	Communicator::GetInstance().SpawnedCharacterDetails.Empty();
}

void AEnemyCharacter::SpawnLoadedActors()
{
	/* Spawn previously saved characters from savefile. */

	FActorSpawnParameters SpawnParams;
	auto ActorToSpawn = Communicator::GetInstance().EnemyCharacterBlueprint;

	// Iterate over array and saved spawn actors.
	for (auto Details : Communicator::GetInstance().SpawnedCharacterDetails)
	{
		FTransform ActorTransform = Details.Transform;
		auto Spawned = Communicator::GetInstance().World->SpawnActor<AEnemyCharacter>(ActorToSpawn, ActorTransform, SpawnParams);
		auto MorphMgr = Spawned->MorphManager;
		if (MorphMgr)
		{
			MorphMgr->LoadFace(&Details);
		}
		
	}
}