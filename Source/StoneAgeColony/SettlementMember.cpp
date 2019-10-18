// Fill out your copyright notice in the Description page of Project Settings.

#include "SettlementMember.h"
//#include "EnemyAI.h"
//#include "SettlementMemberAI.h"
#include "UnoccupiedAI.h"
#include "StationWorkerAI.h"
#include "MorningRegularAI.h"
#include "NightRegularAI.h"
#include "Communicator.h"
#include "EquipmentManager.h"
#include "ObjectFactory.h"
#include "MorphManager.h"
#include "Perception/PawnSensingComponent.h"
#include "Settlement.h"
#include "Structure.h"
#include "House.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "StoneAgeColonyCharacter.h"
#include "DialogueMenu.h"
#include "DialogueChoiceButton.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "SettlementMemberProfession.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SurvivalGameState.h"

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

	Profession = USettlementMemberProfession::GetProfession(EProfession::VE_Unoccupied); // Currently we just have normal set to all settlement members. More will be added later on.
	Name = "Harambe";
}


void ASettlementMember::BeginPlay()
{
    Super::BeginPlay();

	SetupAIController();
	SetupBelongingSettlement();
	 
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

void ASettlementMember::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{

}

void ASettlementMember::OnUsed(APawn* InstigatorPawn)
{
	StartDialogue(InstigatorPawn);
	//UE_LOG(LogTemp, Warning, TEXT("ASettlementMember::OnUsed MY SPECIAL ID: %s"), *SpecialID);
	//Act();
	if (BelongingSettlement)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASettlementMember::OnUsed HAS Settlement."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ASettlementMember::OnUsed no settlement bro wtf you homeless bitch faggot."));
	}

	SetupHome();
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

void ASettlementMember::SetupHome()
{
	// Do nothing if this character already has a home 
	if (Home) 
	{
		UE_LOG(LogTemp, Warning, TEXT("ASettlementMember::SetupHome Already has a home, exiting method."));
		return;
	}


	if (BelongingSettlement)
	{
		for (auto Structure : BelongingSettlement->Structures)
		{
			auto ProbableHome = (AHouse*)Structure;
			if (ProbableHome)
			{
				// if this character has a home from saved game, try to assign to it first
				if (ProbableHome->SpecialID == HomeSpecialID)
				{
					ProbableHome->Occupants.Add(SpecialID);
					Home = ProbableHome;
					UE_LOG(LogTemp, Warning, TEXT("ASettlementMember::SetupHome Home restored from save game."));
					return;
				}

				// if this house has empty space
				if (ProbableHome->Occupants.Num() < ProbableHome->Capacity)
				{
					ProbableHome->Occupants.Add(SpecialID);
					HomeSpecialID = ProbableHome->SpecialID;
					Home = ProbableHome;
					UE_LOG(LogTemp, Warning, TEXT("ASettlementMember::SetupHome New home set, exiting method."));
					return;
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("ASettlementMember::SetupHome no house available bitch."));
}

void ASettlementMember::ChangeProfession(FProfession NewProfession)
{
	Profession = NewProfession;
	SetupAIController();
}

void ASettlementMember::Act()
{
	AIController->Act();
}

void ASettlementMember::GetNotification()
{
	Act();
}

void ASettlementMember::StartDialogue(APawn* InstigatorPawn)
{
	AStoneAgeColonyCharacter* Player = (AStoneAgeColonyCharacter*)InstigatorPawn;
	if (Player)
	{
		if (AIController)
		{
			Controller->StopMovement();
			AIController->Activity = EActivity::VE_Talking;
			
			// Face Player
			auto x = Player->GetActorRotation();
			x.Yaw += 180.f;
			SetActorRotation(x);

		}

		auto DialogueMenu = (UDialogueMenu*)Player->OpenMenu(DialogueMenuRef, NULL, NULL);
		DialogueMenu->Owner = this;
		DialogueMenu->StartingChoiceIDs = { 20000 };
		DialogueMenu->InitialSetup();
		AIController->Activity = EActivity::VE_Talking;
	}
}


void ASettlementMember::SetupAIController()
{
	/* Also called in ASurvivalGameState::UpdateNPCAI on Time of Day changes (morning to afternoon etc) to switch between different AI types */

	// Destroy old AI instance if exists
	if (AIController)
	{
		AIController->StopTimerHandle();
		AIController->Destroy();
	}

	// Copied from SpawnDefaultController() ue4 source code
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = Instigator;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.OverrideLevel = GetLevel();
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save AI controllers into a map


	// Select morning ai if it is morning
	auto GameState = GetWorld()->GetGameState<ASurvivalGameState>();
	if (GameState)
	{
		auto TimeOfDay = GameState->GetTimeOfDay();

		// if before 8 am
		if ((TimeOfDay < 60.f * 8.f) && (TimeOfDay > 60.f * 6.f))
		{			
			UE_LOG(LogTemp, Warning, TEXT("ASettlementMember::SetupAIController Choosing Morning AI"));
			AIControllerClass = AMorningRegularAI::StaticClass();
			AIController = (AMorningRegularAI*)GetWorld()->SpawnActor<AController>(AIControllerClass, GetActorLocation(), GetActorRotation(), SpawnInfo);
			if (AIController != nullptr)
			{
				AIController->Possess(this);
			}
			return;
		}

		// if between 10pm - 6 am
		if (TimeOfDay < 60.f * 6.f || TimeOfDay > 60.f * 22.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("ASettlementMember::SetupAIController Choosing NIGHT AI"));
			AIControllerClass = ANightRegularAI::StaticClass();
			AIController = (ANightRegularAI*)GetWorld()->SpawnActor<AController>(AIControllerClass, GetActorLocation(), GetActorRotation(), SpawnInfo);
			if (AIController != nullptr)
			{
				AIController->Possess(this);
			}
			return;
		}

	}

	UE_LOG(LogTemp, Warning, TEXT("ASettlementMember:: SetupAIController Profession name: %s"), *Profession.ProfessionName);
	if (Profession.Type == EProfession::VE_Unoccupied)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASettlementMember::SetupAIController Choosing Unoccupied AI"));
		AIControllerClass = AUnoccupiedAI::StaticClass();
		AIController = (AUnoccupiedAI*)GetWorld()->SpawnActor<AController>(AIControllerClass, GetActorLocation(), GetActorRotation(), SpawnInfo);
	}
	else if (Profession.Type == EProfession::VE_StoneWorker)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASettlementMember::SetupAIController Choosing StationWorker AI"));
		AIControllerClass = AStationWorkerAI::StaticClass();
		AIController = (AStationWorkerAI*)GetWorld()->SpawnActor<AController>(AIControllerClass, GetActorLocation(), GetActorRotation(), SpawnInfo);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ASettlementMember::SetupAIController Choosing else"));
		AIControllerClass = AUnoccupiedAI::StaticClass();
		AIController = (AUnoccupiedAI*)GetWorld()->SpawnActor<AController>(AIControllerClass, GetActorLocation(), GetActorRotation(), SpawnInfo);
	}
	
	if (AIController != nullptr)
	{
		AIController->Possess(this);
	}
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
	CharDetails.HomeSpecialID = HomeSpecialID;
	CharDetails.ProfessionType = Profession.Type;
	
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
		Spawned->HomeSpecialID = Details.HomeSpecialID;
		Spawned->SetupHome(); //this causes crash, but only sometimes wtf?
		Spawned->ChangeProfession(USettlementMemberProfession::GetProfession(Details.ProfessionType));
		Spawned->Act();
	
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