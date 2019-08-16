// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "GatherableObject.generated.h"

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API AGatherableObject : public AUsableActor
{
	GENERATED_BODY()

public:
	AGatherableObject(const FObjectInitializer& ObjectInitializer);

	virtual void OnUsed(APawn* InstigatorPawn) override;
	
	//UStaticMeshComponent* MeshComp;
	//UStaticMesh* Mesh;
};
