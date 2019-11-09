// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"

class AHumanCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STONEAGECOLONY_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddItem(int32, int32);
	bool Contains(int32);
	void Emplace(int32, int32);
	void ConsumeItem(int32 ItemID, int32 Amount);
	void PrintInventory();
	TMap<int32, int32> GetItems();

	AHumanCharacter* Owner;

	TMap<int32, int32> Items = { {10000, 1}, {400,2}, {401,5}, {402, 5}, {506, 250}, {490,2}, {450, 3}, {700, 10}, {502, 500} };
	

		
};
