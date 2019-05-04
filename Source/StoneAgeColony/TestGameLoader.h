// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "TestGameLoader.generated.h"

/**
 * 
 */

// THIS CLASS IS FOR TEST ONLY, WILL BE DELETED LATER
UCLASS()
class STONEAGECOLONY_API ATestGameLoader : public AUsableActor
{
	GENERATED_BODY()

protected:
	virtual void OnUsed(APawn* InstigatorPawn) override;
	
};
