// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "MorphManager.h"
#include "GameFramework/Character.h"
#include "Equipment.h"
#include "SettlementMemberProfession.h"
#include "SettlementMemberProfession.h"
#include "HumanCharacter.generated.h"


class UEquipmentManager;
class UMorphManager;

USTRUCT(BlueprintType)
struct FHumanCharacterDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialID")
	FString SpecialID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HomeSpecialID")
	FString HomeSpecialID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProfessionName")
	EProfession ProfessionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FaceDetails")
	TMap<FName, float> FaceDetails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	TMap<EEquipmentSlots, int32> EquippedItems;
};

UCLASS()
class STONEAGECOLONY_API AHumanCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHumanCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY()
	FString SpecialID;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void RegisterActorDetailsToSave();

	// Equipment Mesh Components
	USkeletalMeshComponent* UpperHeadEquipmentComponent;
	USkeletalMeshComponent* TorsoEquipmentComponent;
	USkeletalMeshComponent* BeltEquipmentComponent;
	USkeletalMeshComponent* LegsEquipmentComponent;
	USkeletalMeshComponent* FeetEquipmentComponent;
	USkeletalMeshComponent* Ring1EquipmentComponent;
	USkeletalMeshComponent* Ring2EquipmentComponent;

	// Equipment Type-Component Mapping
	TMap<EEquipmentSlots, USkeletalMeshComponent*> EquipmentMapping;

	UPROPERTY()
	FHumanCharacterDetails CharDetails;
	UPROPERTY()
	UMorphManager* MorphManager;
	UPROPERTY()
	UEquipmentManager* EquipmentManager;
	UPROPERTY()
	TMap<EEquipmentSlots, int32> EquippedItems;
	
private:
	void CreateSubObjectMesh(USkeletalMeshComponent* Comp, FName Name);
};
