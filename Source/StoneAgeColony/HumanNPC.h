// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HumanAIController.h"
#include "HumanNPC.generated.h"


UCLASS()
class STONEAGECOLONY_API AHumanNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHumanNPC();
	float PitchValue;
	float YawValue;
	float RollValue;
	float RunningTime;
	
	FVector ForwardVector;
	AHumanAIController* ai;

	UPROPERTY(EditAnywhere, Category = "TestLocationVariables")
	float xLoc;

	UPROPERTY(EditAnywhere, Category = "TestLocationVariables")
	float yLoc;

	UPROPERTY(EditAnywhere, Category = "TestLocationVariables")
	float zLoc;

	UPROPERTY(EditAnywhere, Category = "Identification")
	int id;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Act();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void MoveForward(float);
	virtual void Rotate(float);
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	UAnimMontage* Animation;

	FVector NewLocation;

private:
	FTimerHandle UnusedHandle;
};
