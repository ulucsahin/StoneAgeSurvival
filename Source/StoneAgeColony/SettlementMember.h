// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HumanCharacter.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "SettlementMemberProfession.h"
#include "SettlementMember.generated.h"

class ASettlementMemberAI;
class ASettlement;
//struct FProfession;

UCLASS()
class STONEAGECOLONY_API ASettlementMember : public AHumanCharacter
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// AI Senses
	UFUNCTION()
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

public:
	// Sets default values for this character's properties
	ASettlementMember(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = "Behavior")
	class UBehaviorTree *BotBehavior;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void OnUsed(APawn* InstigatorPawn);
	void StartDialogue(APawn* InstigatorPawn);
	void GetNotification();
	void SetupAIController();

	// Save-Load Methods
	virtual void RegisterActorDetailsToSave() override;
	static void EmptyCommunicatorDetailsArray();
	static void SpawnLoadedActors();
	
	//UPROPERTY(EditAnywhere, Category = "Identification")
	//int ExternalID;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UPawnSensingComponent* PawnSensingComp;

	void SetupBelongingSettlement();
	void ChangeProfession(FProfession NewProfession);
	void Act();

	class UDataTable* PropertiesDataTable;
	FProfession Profession;
	FString Name;
	ASettlement* BelongingSettlement;
	FString LastWorkingStationSpecialID;
	ASettlementMemberAI* AIController;
private:
	float FollowRadius;
	FString DialogueMenuRef;
	//UStaticMeshComponent* PerceptiveArea;
};
