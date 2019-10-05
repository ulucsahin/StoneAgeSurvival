// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIItemSlot.h"
#include "DialogueChoiceButton.generated.h"

class UTextBlock;
class UDialogueMenu;


enum class EButtonTypes : uint8
{
	VE_Profession  UMETA(DisplayName = "Profession"), // Sets Profession
	VE_String   UMETA(DisplayName = "String") // Contains information as string, for example profession

};

USTRUCT(BlueprintType)
struct FLinkedListDialogueItem
{
	GENERATED_USTRUCT_BODY()
public:
	EButtonTypes Type;
	int32 ID;
	FString Text;
	TArray<FLinkedListDialogueItem*> Next;
	TArray<FLinkedListDialogueItem*> Previous;

};

UCLASS()
class STONEAGECOLONY_API UDialogueChoiceButton : public UUIItemSlot
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "ChoiceText")
	UTextBlock* ChoiceTextBox;

	UFUNCTION(BlueprintCallable, Category = "ButtonMouseEvents")
	void OnButtonClick();

	UFUNCTION(BlueprintCallable, Category = "ButtonMouseEvents")
	void OnButtonHover();

	virtual void InitialSetup() override;

	void SetOwnerMemberProfession();

	UDialogueMenu* OwnerDialogueMenu;
	FLinkedListDialogueItem* Data;
	
};
