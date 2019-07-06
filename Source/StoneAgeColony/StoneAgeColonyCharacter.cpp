
#include "StoneAgeColonyCharacter.h"
#include "StoneAgeColonyProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/TimelineComponent.h"

#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "DrawDebugHelpers.h"
#include "SurvivalGameInstance.h"
#include "UsableActor.h"
#include "Communicator.h"
#include "Building.h"
#include "BuildingManager.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

//DECLARE_DYNAMIC_MULTICAST_DELAGATE_OneParam(F)

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

AStoneAgeColonyCharacter::AStoneAgeColonyCharacter()
{	
	// Flag specifying if character can have focus on a new object or not
	bHasNewFocus = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create box component to get nearby items when necessary
	CollisionSphereRadius = 500.f;
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	Sphere->SetSphereRadius(CollisionSphereRadius);
	Sphere->SetCollisionProfileName("OverlapAll");
	Sphere->SetupAttachment(GetCapsuleComponent());
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AStoneAgeColonyCharacter::OnOverlapBegin);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AStoneAgeColonyCharacter::OnOverlapEnd);

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	/*Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);*/

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;

	// Init Stats
	Health = 100.f;
	Stamina = 100.f;
	Gold = 50;
	Experience = 0;
	Level = 1;

	// Set default player state
	PlayerStates = EPlayerStates::VE_Combat;

	// Initialize BuildingManager
	BuildingManager = NewObject<UBuildingManager>();
	BuildingManager->SetWorld(GetWorld());
	BuildingManager->Player = this;
	BuildingManager->AddToRoot(); // TODO: Fix this

	// Initialize PickupManager
	PickupManager = NewObject<APickupManager>();
	PickupManager->SetWorld(GetWorld());
	PickupManager->SetPlayer(this);
	PickupManager->AddToRoot(); // stupid gc

	InventoryOn = false;
	InitializeWidgets();
}

void AStoneAgeColonyCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	//FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));


	UE_LOG(LogTemp, Warning, TEXT("PLAYER GOLD: %d"), GetGold());

}

void AStoneAgeColonyCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	//HandleFocus();
}

void AStoneAgeColonyCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(OtherComp);
		if (Mesh != nullptr)
		{
			ABuilding* Building = Cast<ABuilding>(OtherActor);
			if (Building != nullptr && Building->GetIsBuilt())
			{
				BuildingManager->BuildingsNearPlayer.Add(Building);
				//if (!bOverlapping)
				//	OnOverlappingBegin();
			}
		}
	}
	
}

void AStoneAgeColonyCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this)
	{
		UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(OtherComp);
		if (Mesh != nullptr)
		{
			ABuilding* Building = Cast<ABuilding>(OtherActor);
			if (Building && Building->GetIsBuilt())
			{
				if (BuildingManager->BuildingsNearPlayer.Contains(Building))
				{
					BuildingManager->BuildingsNearPlayer.Remove(Building);
					//if (OverlappingBuildings.Array().Num() <= 0)
					//	OnOverlappingEnd();
				}
			}
		}
	}
}

void AStoneAgeColonyCharacter::HandleFocus() {
	if (Controller && Controller->IsLocalController()) {

		// Get currently usable item we are looking at
		AUsableActor* Usable = GetActorInView<AUsableActor>(250.f);

		// End Focus
		if (FocusedUsableActor != Usable) 
		{
			if (FocusedUsableActor) 
			{
				FocusedUsableActor->OnEndFocus();
			}

			bHasNewFocus = true;
		}

		// Assign new Focus
		FocusedUsableActor = Usable;

		// Start Focus
		if (Usable) 
		{
			if (bHasNewFocus) 
			{
				Usable->OnBeginFocus();
				// Can't have new focus because we already have focus on a Usable.
				bHasNewFocus = false;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AStoneAgeColonyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AStoneAgeColonyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AStoneAgeColonyCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AStoneAgeColonyCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AStoneAgeColonyCharacter::LookUpAtRate);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AStoneAgeColonyCharacter::OnClick);

	// Bind use event
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &AStoneAgeColonyCharacter::Use);

	// Bind open inventory event
	PlayerInputComponent->BindAction("OpenInventory", IE_Pressed, this, &AStoneAgeColonyCharacter::OpenInventory);

	// Bind open character menu event
	PlayerInputComponent->BindAction("OpenCharacterMenu", IE_Pressed, this, &AStoneAgeColonyCharacter::OpenCharacterMenu);

	// Switch between building state and normal state
	PlayerInputComponent->BindAction("ChangeState", IE_Pressed, this, &AStoneAgeColonyCharacter::ChangeState);

	PlayerInputComponent->BindAction("ScrollUp", IE_Pressed, this, &AStoneAgeColonyCharacter::ScrollUp);
	PlayerInputComponent->BindAction("ScrollDown", IE_Pressed, this, &AStoneAgeColonyCharacter::ScrollDown);

	PlayerInputComponent->BindAction("RightClick", IE_Pressed, this, &AStoneAgeColonyCharacter::RightClick);
	PlayerInputComponent->BindAction("ShiftRightClick", IE_Pressed, this, &AStoneAgeColonyCharacter::ShiftRightClick);
	
	PlayerInputComponent->BindAction("PickupActor", IE_Pressed, this, &AStoneAgeColonyCharacter::InteractPickup);

	// Debug event
	PlayerInputComponent->BindAction("DEBUG", IE_Pressed, this, &AStoneAgeColonyCharacter::Debug);
}

void AStoneAgeColonyCharacter::OnClick()
{
	if (PlayerStates == EPlayerStates::VE_Combat)
	{
		// For debug
		if (Health > 0.f)
		{
			Health -= 1.f;
			UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);
		}
	}
	else if (PlayerStates == EPlayerStates::VE_Building)
	{
		if (BuildingManager)
		{
			bool Success = BuildingManager->CompleteBuilding();
			if (Success)
			{
				// start a new building if previous building succesfully built
				BuildingManager->StartBuilding();
			}
			
		}
		
	}
	else if (PlayerStates == EPlayerStates::VE_Pickup)
	{
		InteractPickup();
	}
	
}

void AStoneAgeColonyCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AStoneAgeColonyCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AStoneAgeColonyCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AStoneAgeColonyCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

/*
	Performs ray-trace to find closest looked-at UsableActor.
*/

void AStoneAgeColonyCharacter::InitializeWidgets()
{
	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Initialize Inventory Widget
	//FStringClassReference MyWidgetClassRef(TEXT("/Game/Uluc/HUD/PlayerInventory.PlayerInventory_C"));
	FStringClassReference MyWidgetClassRef(TEXT("/Game/Uluc/HUD/Inventory/PlayerInventory.PlayerInventory_C"));
	UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>();
	InventoryWidget = CreateWidget<UUserWidget>(PlayerController, MyWidgetClass);

	FStringClassReference CharacterMenuWidgtClassRef(TEXT("/Game/Uluc/HUD/CharacterMenu/CharacterMenu.CharacterMenu_C"));
	UClass* CharacterMenuWidgtClass = CharacterMenuWidgtClassRef.TryLoadClass<UUserWidget>();
	CharacterMenuWidget = CreateWidget<UUserWidget>(PlayerController, CharacterMenuWidgtClass);
}

template<typename T>
T* AStoneAgeColonyCharacter::GetActorInView(float Range)
{
	bool debug = false;
	FVector CamLoc;
	FRotator CamRot;

	if (Controller == NULL)
		return NULL;

	/* This retrieves are camera point of view to find the start and direction we will trace. */
	Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * Range);

	FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableActor")), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = false;

	/* FHitResults is passed in with the trace function and holds the result of the trace. */
	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	// log
	if (debug)
	{
		UE_LOG(LogTemp, Warning, TEXT("Camera:, %f, %f, %f"), CamLoc.X, CamLoc.Y, CamLoc.Z);
		UE_LOG(LogTemp, Warning, TEXT("TraceEnd:, %f, %f, %f"), TraceEnd.X, TraceEnd.Y, TraceEnd.Z);

		bool isHit = ActorLineTraceSingle(Hit, TraceStart, TraceEnd, ECC_WorldStatic, TraceParams);
		UE_LOG(LogTemp, Warning, TEXT("HitResult:, %s"), isHit ? TEXT("true") : TEXT("False"));
	}

	/* Uncomment this to visualize your line during gameplay. */
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, true, 1.0f);
	
	//auto test = Cast<ABuilding>(Hit.GetActor());
	//if (test)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Building xD"));
	//}


	return Cast<T>(Hit.GetActor());
}

void AStoneAgeColonyCharacter::Use() 
{
	AUsableActor* Usable = GetActorInView<AUsableActor>(250.f);
	if (Usable)
	{
		Usable->OnUsed(this);
	}
}

void AStoneAgeColonyCharacter::OpenInventory()
{	
	// BE CAREFUL:
	// Constantly creating new InventoryWidget and adding to viewport --> memory leak or automatically deleted?
	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!InventoryOn)
	{
		// Open Inventory
		FStringClassReference MyWidgetClassRef(TEXT("/Game/Uluc/HUD/Inventory/PlayerInventory.PlayerInventory_C"));
		UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>();
		InventoryWidget = CreateWidget<UUserWidget>(PlayerController, MyWidgetClass);
		InventoryWidget->AddToViewport();
		if (PlayerController)
		{
			PlayerController->SetInputMode(FInputModeGameAndUI());
			PlayerController->bShowMouseCursor = true;
		}
		
		InventoryOn = true;
	}
	
	else
	{
		// Close Inventory
		InventoryWidget->RemoveFromParent();
		if (PlayerController)
		{
			PlayerController->SetInputMode(FInputModeGameOnly());
			PlayerController->bShowMouseCursor = false;
		}
		
		InventoryOn = false;
	}
}

void AStoneAgeColonyCharacter::OpenCharacterMenu()
{
	// BE CAREFUL:
	// Constantly creating new InventoryWidget and adding to viewport --> memory leak or automatically deleted?
	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!CharacterMenuOn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Character menu opened."));
		FStringClassReference CharacterMenuWidgtClassRef(TEXT("/Game/Uluc/HUD/CharacterMenu/CharacterMenu.CharacterMenu_C"));
		UClass* CharacterMenuWidgtClass = CharacterMenuWidgtClassRef.TryLoadClass<UUserWidget>();
		CharacterMenuWidget = CreateWidget<UUserWidget>(PlayerController, CharacterMenuWidgtClass);
		CharacterMenuWidget->AddToViewport();
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->bShowMouseCursor = true;
		CharacterMenuOn = true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Character menu closed."));
		CharacterMenuWidget->RemoveFromParent();
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
		CharacterMenuOn = false;
	}
	//'/Game/Uluc/HUD/CharacterMenu/CharacterMenu.CharacterMenu_C'
}

void AStoneAgeColonyCharacter::RegisterSaveData() {
	// Handled in ObjectBed and GameLoader
}

float AStoneAgeColonyCharacter::GetHealth()
{
	return Health;
}

float AStoneAgeColonyCharacter::GetStamina()
{
	return Stamina;
}

int AStoneAgeColonyCharacter::GetGold()
{
	return Gold;
}

int AStoneAgeColonyCharacter::GetExperience()
{
	return Experience;
}

int AStoneAgeColonyCharacter::GetLevel()
{
	return Level;
}

//AUsableActor* AStoneAgeColonyCharacter::GetInventoryItem(int InventoryItemIndex)
//{
//	int ItemIDatIndex = Inventory[InventoryItemIndex];
//	return Communicator::GetInstance().UsableItemIDMap[ItemIDatIndex];
//}

TMap<int, int> AStoneAgeColonyCharacter::GetInventory()
{
	return Inventory;
}

void AStoneAgeColonyCharacter::AddToInventory(int ItemToAdd, int AmountToAdd)
{
	/*ItemToAdd is ID of item that we want to add to inventory.*/

	//Inventory.Add(ItemToAdd);

	// if we have atleast one instance of ItemToAdd in inventory
	if (!Inventory.Contains(ItemToAdd))
	{
		UE_LOG(LogTemp, Warning, TEXT("Item not in inventory"));
		Inventory.Add(ItemToAdd, AmountToAdd);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item already in inventory"));
		Inventory.Emplace(ItemToAdd, Inventory[ItemToAdd] + AmountToAdd);
	}

	for (auto item : Inventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryVersion3 item: %d, amount: %d"), item.Key, item.Value);
	}
	
}

// Called on key press
void AStoneAgeColonyCharacter::ChangeState()
{
	UE_LOG(LogTemp, Warning, TEXT("Change State"));

	if (PlayerStates == EPlayerStates::VE_Building)
	{
		ChangeState(EPlayerStates::VE_Combat);
		UE_LOG(LogTemp, Warning, TEXT("New State: %s"), TEXT("Combat"));
	}
	else if (PlayerStates == EPlayerStates::VE_Combat)
	{
		ChangeState(EPlayerStates::VE_Building);
		UE_LOG(LogTemp, Warning, TEXT("New State: %s"), TEXT("Building"));
	}
	

}

void AStoneAgeColonyCharacter::ChangeState(EPlayerStates NewState)
{
	if (NewState != PlayerStates)
	{
		PlayerStates = NewState;
		UpdateStateDisplay();
	}

}


void AStoneAgeColonyCharacter::UpdateStateDisplay()
{
	// Update display such as UI in here
	if (PlayerStates == EPlayerStates::VE_Building)
	{
		BuildingManager->StartBuilding();
	}
	else if (PlayerStates == EPlayerStates::VE_Combat)
	{
		BuildingManager->CancelBuilding();
	}
}

void AStoneAgeColonyCharacter::ScrollUp()
{
	if (PlayerStates == EPlayerStates::VE_Building)
	{
		BuildingManager->IncreaseForwardBuildingOffset();
	}
	else if (PlayerStates == EPlayerStates::VE_Pickup)
	{
		PickupManager->IncreaseForwardBuildingOffset();
	}
		
}

void AStoneAgeColonyCharacter::ScrollDown()
{
	if (PlayerStates == EPlayerStates::VE_Building)
	{
		BuildingManager->DecreaseForwardBuildingOffset();
	}
	else if (PlayerStates == EPlayerStates::VE_Pickup)
	{
		PickupManager->DecreaseForwardBuildingOffset();
	}
}

void AStoneAgeColonyCharacter::RightClick()
{
	if (PlayerStates == EPlayerStates::VE_Building)
	{
		BuildingManager->IncreaseRotationOffset();
	}
}

void AStoneAgeColonyCharacter::ShiftRightClick()
{
	if (PlayerStates == EPlayerStates::VE_Building)
	{
		//BuildingManager->DecreaseRotationOffset();
		BuildingManager->ChangeBuildingType();
	}
}

// Z Button
void AStoneAgeColonyCharacter::InteractPickup()
{
	UE_LOG(LogTemp, Warning, TEXT("Pickup Actor"));
	
	// Don't do it while in building mode
	if (PlayerStates != EPlayerStates::VE_Building)
	{
		AUsableActor* Usable = GetActorInView<AUsableActor>(250.f);
		bool EnteredState = PickupManager->HandlePickup(Usable);
		if (EnteredState)
		{
			PlayerStates = EPlayerStates::VE_Pickup;
		}
		else
		{
			PlayerStates = EPlayerStates::VE_Combat;
		}
	}
	

}

void AStoneAgeColonyCharacter::StartBuilding()
{
	//ABuilding* test = BuildingManager->StartBuilding();
}



void AStoneAgeColonyCharacter::Debug()
{
	//GetActorInView<ABuilding>(20.f); // WE NEED THIS LINE OR LINKER ERROR CAUSED BY OPTIONS IN STONEAGECOLONY.BUILD.CS WHICH IS USED FOR FASTER COMPILE TIMES
	UE_LOG(LogTemp, Warning, TEXT("hehe debugg"));
	
}

