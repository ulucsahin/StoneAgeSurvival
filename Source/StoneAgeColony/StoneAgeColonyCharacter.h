// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HumanCharacter.h"
#include "StoneAgeColonyCharacter.generated.h"

class UInputComponent;
class UBuildingManager; 
class UFPAnimationManager;
class UUIBottomBar;
class UUIPlayerInventory;
class ASettlement;
class AUsableActor;
class APickupManager;
class USphereComponent;
class UUserWidget;
class USurvivalWidget;
class AStructure;
class ASettlementMember;
class UInventory;
class UInterfaceManager;

enum class EPlayerStates : uint8
{
	VE_Combat 	UMETA(DisplayName = "Combat"),
	VE_Building UMETA(DisplayName = "Building"),
	VE_Pickup   UMETA(DisplayName = "Pickup"),
	VE_Talking  UMETA(DisplayName = "Talking"),
};


UCLASS(config=Game)
class AStoneAgeColonyCharacter : public AHumanCharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* VR_Gun;

	/** Location on VR gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* VR_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* L_MotionController;

public:
	AStoneAgeColonyCharacter(const FObjectInitializer& ObjectInitializer);

	template <typename T>
	T* GetActorInView(float Range);

	USphereComponent* Sphere;
	float CollisionSphereRadius;

	class AUsableActor* FocusedUsableActor;
	void RegisterSaveData();

	// States operations
	EPlayerStates PlayerStates;
	void ChangeState(EPlayerStates NewState);
	void UpdateStateDisplay();

	// Building System Variables
	UBuildingManager* BuildingManager;
	APickupManager* PickupManager;

	// Animation Manager
	UFPAnimationManager* AnimationManager;

	// Menu Flags
	bool InventoryOn = false;
	bool CharacterMenuOn = false;
	bool Init = false;

	// UI Bottom Bar
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInterface")
	UUIBottomBar* BottomBar;

	/*Max distance to use/focus on actors. */
	UPROPERTY(EditDefaultsOnly, Category = "ObjectInteraction")
	float MaxUseDistance;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Stats
	float Health;
	float Stamina;
	int Gold;
	int Experience;
	int Level;

	// UI Functions
	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetHealth();

	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetStamina();

	UFUNCTION(BlueprintPure, Category = "Stats")
	FString GetSettlementBuildingCount();

	UFUNCTION(BlueprintPure, Category = "Stats")
	FString GetSettlementPopulation();

	UFUNCTION(BlueprintPure, Category = "Stats")
	int GetLevel();

	// UI Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainMenu")
	UUserWidget* MainMenuWidget; // used in blueprints

	ASettlement* ActiveSettlement;

	void Debug();

protected:
	bool bHasNewFocus;

	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AStoneAgeColonyProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* IdleAnimationEmptyHands;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint32 bUsingMotionControllers : 1;

	// Opens Given UI
	USurvivalWidget* OpenMenu(FString Reference, AStructure* OwnerStructure, ASettlement* OwnerSettlement);

private:
	void HandleFocus();

protected:

	// Uses looked item
	void Use();

	// Gathers looked item
	void Gather();

	// Opens inventory ui
	void OpenInventory();

	// Opens Character Menu ui
	void OpenCharacterMenu();
	
	/** Fires a projectile. */
	void OnClick();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void ChangeState();

	void ScrollUp();
	void ScrollDown();
	void ShiftScrollUp();
	void ShiftScrollDown();
	void RightClick();
	void ShiftRightClick();
	void InteractPickup();
	

	void StartBuilding();

	UFUNCTION(BlueprintCallable, Category = "FirstPerson")
	void HideFirstPersonHands(bool Hide);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	void InteractPickup(AUsableActor* Actor);

	void CloseAllMenus();

	UPROPERTY()
	TArray<USurvivalWidget*> OpenedMenus;

	UPROPERTY(EditAnywhere)
	UInterfaceManager* InterfaceManager;

};