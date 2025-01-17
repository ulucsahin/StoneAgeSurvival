// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MorphManager.generated.h"

struct FHumanCharacterDetails;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STONEAGECOLONY_API UMorphManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMorphManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SetupManager(ACharacter* Owner);
	void RandomizeFace();
	void LoadFace(FHumanCharacterDetails* CharacterDetails);
	//void RegisterFaceDetails();

	TMap<FName, float> FaceDetails;
private:
	ACharacter* Owner;
};
