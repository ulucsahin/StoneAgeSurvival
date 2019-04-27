// Fill out your copyright notice in the Description page of Project Settings.

#include "SpherePawn.h"
#include "Classes/Components/InputComponent.h"
#include "Classes/GameFramework/FloatingPawnMovement.h"
#include "Classes/GameFramework/SpringArmComponent.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Classes/Camera/CameraComponent.h"

// Sets default values
ASpherePawn::ASpherePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");

	CameraArm = CreateDefaultSubobject<USpringArmComponent>("CameraSpringArm");
	CameraArm->SetupAttachment(StaticMesh);
	CameraArm->TargetArmLength = 500.f;

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("PawnMovement");
	Camera = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	//Camera->SetRelativeLocation(FVector(-500.f, 0.f, 0.f));
	Camera->SetupAttachment(CameraArm);
	SetRootComponent(StaticMesh);

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
}

// Called when the game starts or when spawned
void ASpherePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpherePawn::MoveForward(float Amount){
	
	FloatingPawnMovement->AddInputVector(GetActorForwardVector() * Amount);
}

void ASpherePawn::MoveRight(float Amount) {
	FloatingPawnMovement->AddInputVector(GetActorRightVector() * Amount);
}

void ASpherePawn::Turn(float Amount){
	AddControllerYawInput(Amount);
	
}

void ASpherePawn::LookUp(float Amount){
	AddControllerPitchInput(Amount);
}

// Called every frame
void ASpherePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASpherePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASpherePawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASpherePawn::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ASpherePawn::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ASpherePawn::LookUp);

}

