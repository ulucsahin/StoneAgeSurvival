// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InterfaceManager.generated.h"

class USurvivalWidget;
class AStructure;
class ASettlement;
class AStoneAgeColonyCharacter;
class UUIPlayerInventory;
class APlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STONEAGECOLONY_API UInterfaceManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInterfaceManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RegisterPlayer(AStoneAgeColonyCharacter* Player);

	USurvivalWidget* OpenMenu(FString Reference, AStructure* OwnerStructure, ASettlement* OwnerSettlement);
	void CloseMenu();
	void CloseAllMenus();

	void OpenCloseInventory();
	void OpenCloseCharacterMenu();

	void SetInputModeAuto();
	void SetInputModeGameOnly();
	void SetInputModeGameAndUI();

	int32 GetNumberOfOpenMenus();

	UPROPERTY()
	TArray<USurvivalWidget*> OpenedMenus;
	
	bool PlayerInventoryOn = false;
	bool PlayerCharacterMenuOn = false;

private:
	UPROPERTY()
	AStoneAgeColonyCharacter* Player;
	UPROPERTY()
	UUIPlayerInventory* InventoryWidget;
	UPROPERTY()
	USurvivalWidget* CharacterMenuWidget;
	UPROPERTY()
	APlayerController* PlayerController;
	
};
