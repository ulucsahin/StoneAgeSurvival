
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

	InventoryOn = false;
	InitializeWidgets();
}

void AStoneAgeColonyCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	//FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	//// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	//if (bUsingMotionControllers)
	//{
	//	VR_Gun->SetHiddenInGame(false, true);
	//	Mesh1P->SetHiddenInGame(true, true);
	//}
	//else
	//{
	//	VR_Gun->SetHiddenInGame(true, true);
	//	Mesh1P->SetHiddenInGame(false, true);
	//}


}

void AStoneAgeColonyCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	HandleFocus();
}

void AStoneAgeColonyCharacter::HandleFocus() {
	if (Controller && Controller->IsLocalController()) {

		// Get currently usable item we are looking at
		AUsableActor* Usable = GetUsableInView();

		// End Focus
		if (FocusedUsableActor != Usable) {
			if (FocusedUsableActor) {
				FocusedUsableActor->OnEndFocus();
			}

			bHasNewFocus = true;
		}

		// Assign new Focus
		FocusedUsableActor = Usable;

		// Start Focus
		if (Usable) {
			if (bHasNewFocus) {
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
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AStoneAgeColonyCharacter::OnFire);

	// Bind use event
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &AStoneAgeColonyCharacter::Use);

	// Bind open inventory event
	PlayerInputComponent->BindAction("OpenInventory", IE_Pressed, this, &AStoneAgeColonyCharacter::OpenInventory);

	// Bind open character menu event
	PlayerInputComponent->BindAction("OpenCharacterMenu", IE_Pressed, this, &AStoneAgeColonyCharacter::OpenCharacterMenu);

	// Debug event
	PlayerInputComponent->BindAction("DEBUG", IE_Pressed, this, &AStoneAgeColonyCharacter::PrintInventory);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AStoneAgeColonyCharacter::OnResetVR);

}

void AStoneAgeColonyCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<AStoneAgeColonyProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<AStoneAgeColonyProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	//if (FireAnimation != NULL)
	//{
	//	// Get the animation object for the arms mesh
	//	UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
	//	if (AnimInstance != NULL)
	//	{
	//		AnimInstance->Montage_Play(FireAnimation, 1.f);
	//	}
	//}

	// For debug
	if (Health > 0.f) 
	{
		Health -= 1.f;
		UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);
	}
		
}

void AStoneAgeColonyCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AStoneAgeColonyCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AStoneAgeColonyCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AStoneAgeColonyCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

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

bool AStoneAgeColonyCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AStoneAgeColonyCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AStoneAgeColonyCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AStoneAgeColonyCharacter::TouchUpdate);
		return true;
	}
	
	return false;
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

AUsableActor* AStoneAgeColonyCharacter::GetUsableInView()
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
	const FVector TraceEnd = TraceStart + (Direction * 250.f);

	// log
	if (debug) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Camera:, %f, %f, %f"), CamLoc.X, CamLoc.Y, CamLoc.Z);
		UE_LOG(LogTemp, Warning, TEXT("TraceEnd:, %f, %f, %f"), TraceEnd.X, TraceEnd.Y, TraceEnd.Z);
	}

	FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableActor")), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = false;

	/* FHitResults is passed in with the trace function and holds the result of the trace. */
	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	/* Uncomment this to visualize your line during gameplay. */
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, true, 1.0f);
	
	return Cast<AUsableActor>(Hit.GetActor());
}

void AStoneAgeColonyCharacter::Use() 
{
	AUsableActor* Usable = GetUsableInView();
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
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->bShowMouseCursor = true;
		InventoryOn = true;
	}
	
	else
	{
		// Close Inventory
		InventoryWidget->RemoveFromParent();
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
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

TArray<int> AStoneAgeColonyCharacter::GetInventory() 
{
	return Inventory;
}

void AStoneAgeColonyCharacter::AddToInventory(int ItemToAdd)
{
	Inventory.Add(ItemToAdd);
}

void AStoneAgeColonyCharacter::PrintInventory()
{
	UE_LOG(LogTemp, Warning, TEXT("AStoneAgeColonyCharacter::PrintInventory"));
	for (int itemID : Inventory) 
	{
		UE_LOG(LogTemp, Warning, TEXT("ITEM IN INVENTORY: %d"), itemID);
		//item->PrintName();
		AUsableActor* CurrentItem = Communicator::GetInstance().UsableItemIDMap[itemID];
		
		//CurrentItem->PrintName();
		//UE_LOG(LogTemp, Warning, TEXT("CurrentItem ID: %d"), CurrentItem->ID);
	}
}