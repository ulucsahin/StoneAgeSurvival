// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "UsableActor.h"
#include "Equipment.generated.h"



UENUM()
enum class EEquipmentSlots : uint8
{
	VE_UpperHead   UMETA(DisplayName = "UpperHead"),
	VE_Torso   UMETA(DisplayName = "Torso"),
	VE_Belt   UMETA(DisplayName = "Belt"),
	VE_Legs  UMETA(DisplayName = "Legs"),
	VE_Feet   UMETA(DisplayName = "Feet"),
	VE_Ring1   UMETA(DisplayName = "Ring1"),
	VE_Ring2   UMETA(DisplayName = "Ring2")
};

USTRUCT(BlueprintType)
struct FEquipmentData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FEquipmentData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	FString Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	int32 Stat1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	TAssetPtr<USkeletalMesh> MeshRef;

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STONEAGECOLONY_API AEquipment : public AUsableActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AEquipment(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts
	//virtual void BeginPlay();// override;

public:	
	// Called every frame
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USkeletalMesh* SkeletalMesh;

	void SetupType(FString);

	int32 GetID() { return ID; };

	// Information from DataTable
	int32 ID;
	EEquipmentSlots EquipmentType;
	int32 Stat1;
private:

	const TMap<FString, EEquipmentSlots> TypeMap = {
		{"UpperHead", EEquipmentSlots::VE_UpperHead},
		{"Torso", EEquipmentSlots::VE_Torso},
		{"Belt", EEquipmentSlots::VE_Belt},
		{"Legs", EEquipmentSlots::VE_Legs},
		{"Feet", EEquipmentSlots::VE_Feet},
		{"Ring1", EEquipmentSlots::VE_Ring1},
		{"Ring2", EEquipmentSlots::VE_Ring2}
	};

	class UDataTable* PropertiesDataTable;
	FEquipmentData* EquipmentData;

	

};