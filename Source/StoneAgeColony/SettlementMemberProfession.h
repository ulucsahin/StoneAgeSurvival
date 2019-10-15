// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structure.h"
#include "SettlementMemberProfession.generated.h"


UENUM()
enum class EProfession : uint8
{
	VE_Unoccupied  UMETA(DisplayName = "Unoccupied"), 
	VE_Carpenter   UMETA(DisplayName = "Carpenter"),
	VE_StoneWorker   UMETA(DisplayName = "StoneWorker"),

};

USTRUCT(BlueprintType)
struct FProfession
{
	GENERATED_USTRUCT_BODY()

public:
	FString ProfessionName = "unoccupied";
	EProfession Type = EProfession::VE_Unoccupied;
	int32 WorkstationTypeID = -1;
	//AStructure* Type = NewObject<AStructure>();
};

USTRUCT(BlueprintType)
struct FUnoccupied : public FProfession
{
	GENERATED_USTRUCT_BODY()

public:
	FUnoccupied()
	{
		ProfessionName = "unoccupied";
		Type = EProfession::VE_Unoccupied;
		WorkstationTypeID = -1;
	}
};

USTRUCT(BlueprintType)
struct FCarpenter : public FProfession
{
	GENERATED_USTRUCT_BODY()

public:
	FCarpenter()
	{
		ProfessionName = "carpenter";
		Type = EProfession::VE_Carpenter;
		WorkstationTypeID = 400;
	}

};

USTRUCT(BlueprintType)
struct FStoneWorker : public FProfession
{
	GENERATED_USTRUCT_BODY()

public:
	FStoneWorker()
	{
		ProfessionName = "stone worker";
		Type = EProfession::VE_StoneWorker;
		WorkstationTypeID = 401;
	}

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STONEAGECOLONY_API USettlementMemberProfession : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USettlementMemberProfession();

	static FProfession GetProfession(EProfession Profession);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
};

