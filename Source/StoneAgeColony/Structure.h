// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "UsableActor.h"
//#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Structure.generated.h"

class UUserWidget;

UCLASS()
class STONEAGECOLONY_API AStructure : public AUsableActor
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnUsed(APawn* InstigatorPawn) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
protected:
	UUserWidget* Menu;

private:
	FName Name;
	
};
