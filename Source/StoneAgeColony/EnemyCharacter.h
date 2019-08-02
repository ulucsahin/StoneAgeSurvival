// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MorphManager.h"
#include "EnemyCharacter.generated.h"

USTRUCT(BlueprintType)
struct FEnemyCharacterDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FaceDetails")
	TMap<FName, float> FaceDetails;
};

UCLASS()
class STONEAGECOLONY_API AEnemyCharacter : public ACharacter
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

	FEnemyCharacterDetails CharDetails;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	class UBehaviorTree *BotBehavior;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Save-Load Methods
	void RegisterActorDetailsToSave();
	static void EmptyCommunicatorDetailsArray();
	//static TArray<FEnemyCharacterDetails> GetCommunicatorDetailsArray();
	//static TSubclassOf<AEnemyCharacter> GetActorToSpawn();
	static void SpawnLoadedActors();
	//static TSubclassOf<AEnemyCharacter> EnemyCharacterBlueprint;

	UPROPERTY(EditAnywhere, Category = "Identification")
	int ExternalID;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UPawnSensingComponent* PawnSensingComp;

private:
	float FollowRadius;
	UMorphManager* MorphManager;
	
};

