// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIItemSlot.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "SettlementMemberProfession.h"
#include "DialogueChoiceButton.generated.h"

class UTextBlock;
class UDialogueMenu;

UENUM()
enum class EButtonTypes : uint32
{
	// Introduction
	VE_Introduction						UMETA(DisplayName = "Introduction"), 
	// Displays profession
	VE_AskJob							UMETA(DisplayName = "AskJob"),  
	// Just displays different choices
	VE_OpenMenu							UMETA(DisplayName = "OpenMenu"), 
	// Opens a user interface menu
	VE_OpenSpecifyCraftingMenu			UMETA(DisplayName = "OpenSpecifyCraftingMenu"),
	// Sets profession
	VE_AssignJob						UMETA(DisplayName = "AssignJob"), 

};

USTRUCT(BlueprintType)
struct FDialogueChoiceData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FDialogueChoiceData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChoiceData")
	FString Label; // just for giving names to buttons (for development purposes)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChoiceData")
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChoiceData")
	EButtonTypes Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChoiceData")
	FString Query; // text that will be displayed on button

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChoiceData")
	FString Payload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChoiceData")
	FString Response;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChoiceData")
	TArray<int32> Next;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChoiceData")
	TArray<int32> Previous;
};

UCLASS()
class STONEAGECOLONY_API UDialogueChoiceButton : public UUIItemSlot
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "ChoiceText")
	UTextBlock* ChoiceTextBox;

	void SetupType(FString ID_as_String);

	UFUNCTION(BlueprintCallable, Category = "ButtonMouseEvents")
	void OnButtonClick();

	UFUNCTION(BlueprintCallable, Category = "ButtonMouseEvents")
	void OnButtonHover();

	virtual void InitialSetup() override;
	void SetOwnerMemberProfession(EProfession Profession);
	FString ActAndReturnResponse();
	EProfession GetJobFromQuery();


	class UDataTable* PropertiesDataTable;
	UDialogueMenu* OwnerDialogueMenu;
	FDialogueChoiceData* ButtonData;
	
	int32 ID;
	EButtonTypes Type;
	FString Query;
	FString Payload;
	FString Response;
	TArray<int32> Next;
	TArray<int32> Previous;
	bool NextButtonsEnabled = true;

};
