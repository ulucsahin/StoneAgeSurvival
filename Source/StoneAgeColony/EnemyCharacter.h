// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HumanCharacter.h"
//#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

//USTRUCT(BlueprintType)
//struct FEnemyCharacterDetails
//{
//	GENERATED_USTRUCT_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
//	FTransform Transform;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FaceDetails")
//	TMap<FName, float> FaceDetails;
//};


UCLASS()
class STONEAGECOLONY_API AEnemyCharacter : public AHumanCharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// AI Senses
	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

	UFUNCTION()
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

public:	
	// Sets default values for this character's properties
	AEnemyCharacter();

	UPROPERTY(EditAnywhere, Category = "Behavior")
	class UBehaviorTree *BotBehavior;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Save-Load Methods
	virtual void RegisterActorDetailsToSave() override;
	static void EmptyCommunicatorDetailsArray();
	static void SpawnLoadedActors();

	UPROPERTY(EditAnywhere, Category = "Identification")
	int ExternalID;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UPawnSensingComponent* PawnSensingComp;

private:
	float FollowRadius;
	
	
};

