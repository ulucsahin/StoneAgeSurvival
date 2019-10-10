// Fill out your copyright notice in the Description page of Project Settings.

#include "SettlementMember.h"
//#include "EnemyAI.h"
#include "SettlementMemberAI.h"
#include "Communicator.h"
#include "EquipmentManager.h"
#include "ObjectFactory.h"
#include "MorphManager.h"
#include "Perception/PawnSensingComponent.h"
#include "Settlement.h"
#include "Structure.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "StoneAgeColonyCharacter.h"
#include "DialogueMenu.h"
#include "DialogueChoiceButton.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "SettlementMemberProfession.h"
#include "GameFramework/CharacterMovementComponent.h"

ASettlementMember::ASettlementMember(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	/* Our sensing component to detect players by visibility and noise checks. */
	//PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	//PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	//PawnSensingComp->SightRadius = 2000;
	//PawnSensingComp->HearingThreshold = 600;
	//PawnSensingComp->LOSHearingThreshold = 1200;
	//FollowRadius = 1500.f;

	//auto AreaDisplayerMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	//auto AreaDisplayerMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/Uluc/Settlement/Materials/AreaDisplayer_Mat.AreaDisplayer_Mat"));

	//PerceptiveArea = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("PerceptiveArea")); // MeshComp from UsableActor parent class
	//PerceptiveArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//PerceptiveArea->SetCollisionProfileName("OverlapAll");
	//PerceptiveArea->SetupAttachment(RootComponent);
	//PerceptiveArea->SetStaticMesh(AreaDisplayerMesh);
	//PerceptiveArea->SetMaterial(0, AreaDisplayerMaterial);
	//PerceptiveArea->SetVisibility(true);
	//PerceptiveArea->OnComponentBeginOverlap.AddDynamic(this, &ASettlement::OnOverlapBegin);
	//PerceptiveArea->OnComponentEndOverlap.AddDynamic(this, &ASettlement::OnOverlapEnd);


	static ConstructorHelpers::FObjectFinder<UDataTable> PropertiesDataObject(TEXT("DataTable'/Game/Uluc/DataTables/DialogueChoiceDataTable.DialogueChoiceDataTable'"));
	if (PropertiesDataObject.Succeeded())
	{
		PropertiesDataTable = PropertiesDataObject.Object;
	}



	auto mesh = GetMesh();
	mesh->SetCollisionProfileName("BlockAll");
	DialogueMenuRef = "'/Game/Uluc/NPC/DialogueSystem/DialogueMenu_BP.DialogueMenu_BP_C'";
	UE_LOG(LogTemp, Warning, TEXT("ASettlementMember:: ASettlementMember"));

	Profession = USettlementMemberProfession::GetProfession("unoccupied"); // Currently we just have normal set to all settlement members. More will be added later on.
	Name = "Harambe";
}


void ASettlementMember::BeginPlay()
{
	Super::BeginPlay();

	/* This is the earliest moment we can bind our delegates to the component */
	//if (PawnSensingComp) {
	//	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASettlementMember::OnSeePlayer);
	//	PawnSensingComp->OnHearNoise.AddDynamic(this, &ASettlementMember::OnHearNoise);
	//}

	 AIController = Cast<ASettlementMemberAI>(GetController());
	 SetupBelongingSettlement();

	 //AIController->MoveToWorkingStation();
	 
	 auto x = GetCharacterMovement();
	 x->MaxWalkSpeed = 100.f;

	 if (SpecialID.Len() < 1)
	 {
		 SpecialID = Communicator::GetInstance().GenerateID();
	 }
}


// Called every frame
void ASettlementMember::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASettlementMember::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void ASettlementMember::OnSeePlayer(APawn* Pawn) 
{
	
	ACharacter* SensedPawn = Cast<ACharacter>(Pawn);

	// Allows AI to stop chasing target when target is out of range
	AIController->SetTargetInRange(true);

	if (AIController /*&& SensedPawn->IsAlive()*/)
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
	if (Distance > FollowRadius) 
	{
		AIController->SetTargetInRange(false);
	}
}

void ASettlementMember::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{

}

void ASettlementMember::SetupBelongingSettlement()
{
	TArray<AActor*> FoundSettlements;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASettlement::StaticClass(), FoundSettlements);

	for (auto Settlement : FoundSettlements)
	{
		if (((ASettlement*)Settlement)->IsActiveSettlement)
		{
			BelongingSettlement = (ASettlement*)Settlement;
			BelongingSettlement->RegisterMember(this);
		}

	}

}

void ASettlementMember::ChangeProfession(FProfession NewProfession)
{
	UE_LOG(LogTemp, Warning, TEXT("ASettlementMember::ChangeProfession"));
	SetupBelongingSettlement();
	Profession = NewProfession;
}

void ASettlementMember::MoveToStation()
{
	AIController->MoveToWorkingStation();
}


void ASettlementMember::OnUsed(APawn* InstigatorPawn)
{
	StartDialogue(InstigatorPawn);
	UE_LOG(LogTemp, Warning, TEXT("MY SPECIAL ID: %s"), *SpecialID);
	UE_LOG(LogTemp, Warning, TEXT("MY SPECIAL ID Len: %d"), SpecialID.Len());
	
}

void ASettlementMember::StartDialogue(APawn* InstigatorPawn)
{
	AStoneAgeColonyCharacter* Player = (AStoneAgeColonyCharacter*)InstigatorPawn;
	if (Player)
	{
		auto DialogueMenu = (UDialogueMenu*)Player->OpenMenu(DialogueMenuRef, NULL, NULL);
		DialogueMenu->Owner = this;
		DialogueMenu->StartingChoiceIDs = { 20000 };
		DialogueMenu->InitialSetup();
	}
}

void ASettlementMember::GetNotification()
{
	MoveToStation();
}

//
// Save-Load methods
//

void ASettlementMember::RegisterActorDetailsToSave()
{
	Super::RegisterActorDetailsToSave();

	// Assign details to struct.
	CharDetails.Transform = GetActorTransform();
	CharDetails.FaceDetails = MorphManager->FaceDetails;
	CharDetails.SpecialID = SpecialID;
	CharDetails.ProfessionName = Profession.ProfessionName;
	
	// Save equipments
	CharDetails.EquippedItems = EquipmentManager->EquippedItems;

	// Save details as struct to communicator. Which will be used during saving.
	Communicator::GetInstance().SpawnedCharacterDetails.Add(CharDetails);
}

void ASettlementMember::EmptyCommunicatorDetailsArray()
{
	Communicator::GetInstance().SpawnedCharacterDetails.Empty();
}

void ASettlementMember::SpawnLoadedActors()
{

	/* Spawn previously saved characters from savefile. */

	FActorSpawnParameters SpawnParams;
	auto ActorToSpawn = Communicator::GetInstance().SettlementMemberBlueprint;

	// Iterate over array and saved spawn actors.
	for (auto Details : Communicator::GetInstance().SpawnedCharacterDetails)
	{
		FTransform ActorTransform = Details.Transform;
		auto Spawned = Communicator::GetInstance().World->SpawnActor<ASettlementMember>(ActorToSpawn, ActorTransform, SpawnParams);

		Spawned->SetupBelongingSettlement();
		Spawned->SpecialID = Details.SpecialID;
		Spawned->Profession = USettlementMemberProfession::GetProfession(Details.ProfessionName);
		Spawned->MoveToStation();
	
		// Restore Morph Settings
		auto MorphMgr = Spawned->MorphManager;
		if (MorphMgr)
		{
			MorphMgr->LoadFace(&Details);
		}

		// Restore Equipments
		auto EquipManager = Spawned->EquipmentManager;
		if (EquipManager)
		{
			AObjectFactory* Factory = NewObject<AObjectFactory>();
			for (auto Item : Details.EquippedItems)
			{
				auto Equipment = (AEquipment*)Factory->CreateObjectBetter(Item.Value); // Item.Value is item ID
				EquipManager->EquipItem(Equipment);
			}
		}

	}
}