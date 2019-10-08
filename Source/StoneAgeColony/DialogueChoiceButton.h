// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIItemSlot.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "DialogueChoiceButton.generated.h"

class UTextBlock;
class UDialogueMenu;


enum class EButtonTypes : uint8
{
	VE_Profession  UMETA(DisplayName = "Profession"), // Sets Profession
	VE_String   UMETA(DisplayName = "String") // Contains information as string, for example profession

};

USTRUCT(BlueprintType)
struct FLinkedListDialogueItem : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FLinkedListDialogueItem() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChoiceData")
	FString Label; // just for giving names to buttons (for development purposes)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChoiceData")
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChoiceData")
	FString Type;

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
	void SetOwnerMemberProfession(FString Profession);
	FString GenerateResponse();
	FString GetJobFromQuery();

	class UDataTable* PropertiesDataTable;
	UDialogueMenu* OwnerDialogueMenu;
	FLinkedListDialogueItem* ButtonData;
	
	int32 ID;
	FString Type;
	FString Query;
	FString Payload;
	FString Response;
	TArray<int32> Next;
	TArray<int32> Previous;

};
