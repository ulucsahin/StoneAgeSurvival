// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HumanCharacter.h"
#include "Equipment.h"
#include "EquipmentManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STONEAGECOLONY_API UEquipmentManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupManager(AHumanCharacter* Owner);
	void EquipItem(UEquipment* Equipment);
	void UnequipItem(EEquipmentSlots ItemSlot);

	TMap<EEquipmentSlots, int32> EquippedItems;
private:
	AHumanCharacter* Owner;
	UEquipment* CurrentEquipment;
	
};
