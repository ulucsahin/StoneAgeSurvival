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
	VE_Farmer   UMETA(DisplayName = "Farmer"),
	VE_Woodcutter UMETA(DisplayName = "WoodCutter"),
	VE_Miner UMETA(DisplayName = "Miner"),

};

USTRUCT(BlueprintType)
struct FProfession
{
	GENERATED_USTRUCT_BODY()

public:
	FString ProfessionName = "unoccupied";
	EProfession Type = EProfession::VE_Unoccupied;
	int32 WorkstationTypeID = -1; // if type of profession that works at crafting stations, npc uses station with this ID
	int32 GatherTypeID = -1; // if type of profession that gathers raw materials, npc gathers from actors with this ID
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

USTRUCT(BlueprintType)
struct FFarmer : public FProfession
{
	GENERATED_USTRUCT_BODY()

public:
	FFarmer()
	{
		ProfessionName = "farmer";
		Type = EProfession::VE_Farmer;
		WorkstationTypeID = 450;
	}

};

USTRUCT(BlueprintType)
struct FWoodCutter : public FProfession
{
	GENERATED_USTRUCT_BODY()

public:
	FWoodCutter()
	{
		ProfessionName = "woodcutter";
		Type = EProfession::VE_Woodcutter;
		GatherTypeID = 100;
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

