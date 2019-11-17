// Fill out your copyright notice in the Description page of Project Settings.

#include "InterfaceManager.h"
#include "SurvivalWidget.h"
#include "Settlement.h"
#include "Structure.h"
//#include "Kismet/GameplayStatics.h"
#include "StoneAgeColonyCharacter.h"
#include "UIPlayerInventory.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UInterfaceManager::UInterfaceManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UInterfaceManager::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UInterfaceManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInterfaceManager::RegisterPlayer(AStoneAgeColonyCharacter* Player)
{
	this->Player = Player;
	this->PlayerController = Cast<APlayerController>(Player->GetController());
}

USurvivalWidget* UInterfaceManager::OpenMenu(FString Reference, AStructure* OwnerStructure, ASettlement* OwnerSettlement)
{
	/* Adds menu to viewport by using String Reference */

	//auto PlayerController = Cast<APlayerController>(Player->GetController());
		
	// Open CraftingStation Menu
	FStringClassReference MyWidgetClassRef(Reference);
	UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<USurvivalWidget>();
	auto MenuWidget = CreateWidget<USurvivalWidget>(PlayerController, MyWidgetClass);
	
	if (MenuWidget)
	{
		if (OwnerStructure)
		{
			MenuWidget->OwnerStructure = OwnerStructure;
			if (OwnerSettlement) MenuWidget->OwnerSettlement = OwnerSettlement;

		}

		MenuWidget->AddToViewport();
		MenuWidget->IsActive = true;

		SetInputModeGameAndUI();

		OpenedMenus.Emplace(MenuWidget);
	}

	return MenuWidget;
}

void UInterfaceManager::CloseMenu()
{

}

void UInterfaceManager::CloseAllMenus()
{
	for (int i = 0; i < OpenedMenus.Num(); i++)
	{
		auto Menu = OpenedMenus[i];
		if (Menu)
		{
			Menu->CloseMenu();
		}
	}

	// Remove any nullptrs too.
	OpenedMenus.Empty();

	SetInputModeGameOnly();

	PlayerInventoryOn = false;
	PlayerCharacterMenuOn = false;
}

void UInterfaceManager::OpenCloseInventory()
{
	// BE CAREFUL:
	// Constantly creating new InventoryWidget and adding to viewport --> memory leak or automatically deleted?

	if (!PlayerInventoryOn)
	{
		// Open Inventory
		InventoryWidget = Cast<UUIPlayerInventory>(OpenMenu(TEXT("/Game/Uluc/HUD/Inventory/PlayerInventory.PlayerInventory_C"), nullptr, nullptr));
		InventoryWidget->InitialSetup(Player);
		PlayerInventoryOn = true;
	}
	else
	{
		InventoryWidget->CloseMenu();
		PlayerInventoryOn = false;
	}
}

void UInterfaceManager::OpenCloseCharacterMenu()
{
	// BE CAREFUL:
	// Constantly creating new InventoryWidget and adding to viewport --> memory leak or automatically deleted?

	//auto PlayerController = Cast<APlayerController>(Player->GetController());

	if (!PlayerCharacterMenuOn)
	{
		CharacterMenuWidget = Cast<USurvivalWidget>(OpenMenu(TEXT("/Game/Uluc/HUD/CharacterMenu/CharacterMenu.CharacterMenu_C"), nullptr, nullptr));
		PlayerCharacterMenuOn = true;
	}
	else
	{
		if (CharacterMenuWidget)
		{
			CharacterMenuWidget->CloseMenu();
		}
		
		PlayerCharacterMenuOn = false;
	}
}

void UInterfaceManager::SetInputModeAuto()
{
	if (GetNumberOfOpenMenus() > 0)
	{
		SetInputModeGameAndUI();
	}
	else
	{
		SetInputModeGameOnly();
	}
}

void UInterfaceManager::SetInputModeGameOnly()
{
	//auto PlayerController = Cast<APlayerController>(Player->GetController());
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
		PlayerController->bEnableClickEvents = false;
		PlayerController->bEnableMouseOverEvents = false;
	}
}

void UInterfaceManager::SetInputModeGameAndUI()
{
	//auto PlayerController = Cast<APlayerController>(Player->GetController());
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
	}
}

int32 UInterfaceManager::GetNumberOfOpenMenus()
{
	/*
	OpenedMenus array contains nullptr for recently closed menus. 
	This method counts amount of menus still open.
	*/

	int32 Result = 0;
	for (int i = 0; i < OpenedMenus.Num(); i++)
	{
		if (OpenedMenus[i] != nullptr) Result++;
	}

	return Result;
}