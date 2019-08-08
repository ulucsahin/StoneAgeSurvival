// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanCharacter.h"
#include "Equipment.h"
#include "EquipmentManager.h"
#include "Communicator.h"
#include "ObjectFactory.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"

// Sets default values
AHumanCharacter::AHumanCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("AHumanCharacter::AHumanCharacter"));

 	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//CreateSubObjectMesh(FeetEquipmentComponent, FName("FeetEquipmentComponent"));

	FeetEquipmentComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FeetEquipmentComponent"));
	FeetEquipmentComponent->SetupAttachment(GetMesh());
	FeetEquipmentComponent->RelativeLocation = FVector::ZeroVector;
	FeetEquipmentComponent->SetMasterPoseComponent(GetMesh());

	LegsEquipmentComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegsEquipmentComponent"));
	LegsEquipmentComponent->SetupAttachment(GetMesh());
	LegsEquipmentComponent->RelativeLocation = FVector::ZeroVector;
	LegsEquipmentComponent->SetMasterPoseComponent(GetMesh());

	//USkeletalMeshComponent* UpperHeadEquipmentComponent;
	//USkeletalMeshComponent* TorsoEquipmentComponent;
	//USkeletalMeshComponent* BeltEquipmentComponent;
	//USkeletalMeshComponent* Ring1EquipmentComponent;
	//USkeletalMeshComponent* Ring2EquipmentComponent;
	// Set equipment type-skeletal mesh component mapping

	EquipmentMapping = {
		{EEquipmentSlots::VE_Feet, FeetEquipmentComponent},
		{EEquipmentSlots::VE_Legs, LegsEquipmentComponent}

	};
	
}

// Called when the game starts or when spawned
void AHumanCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set MorphManager 
	MorphManager = NewObject<UMorphManager>();
	MorphManager->SetupManager(this);
	
	// Set EquipmentManager
	EquipmentManager = NewObject<UEquipmentManager>();
	EquipmentManager->SetupManager(this);

	// TEST: Equipping
	//auto test = NewObject<UEquipment>();
	//test->SetupType("Shoes");
	////EquipmentManager->EquipItem(test);
	//auto test2 = NewObject<UEquipment>();
	//test2->SetupType("Trousers");
	//EquipmentManager->EquipItem(test2);
}

// Called every frame
void AHumanCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHumanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHumanCharacter::RegisterActorDetailsToSave()
{

}

void AHumanCharacter::CreateSubObjectMesh(USkeletalMeshComponent* Comp, FName Name)
{
	Comp = CreateDefaultSubobject<USkeletalMeshComponent>(Name);
	Comp->SetupAttachment(GetMesh());
	Comp->RelativeLocation = FVector::ZeroVector;
	Comp->SetMasterPoseComponent(GetMesh());

	//EquipmentMapping.Emplace(EEquipmentSlots::VE_Feet, Comp);
}