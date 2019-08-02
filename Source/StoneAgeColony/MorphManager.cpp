// Fill out your copyright notice in the Description page of Project Settings.

#include "MorphManager.h"
#include "EnemyCharacter.h"

// Sets default values for this component's properties
UMorphManager::UMorphManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UMorphManager::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UMorphManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
void UMorphManager::SetupManager(ACharacter* Owner)
{
	/* Initial setup when creating new character */
	
	this->Owner = Owner;
	
	RandomizeFace();
}

void UMorphManager::RandomizeFace()
{
	/* Randomizes face features of owner character. Used when creating new character. */
	if (Owner)
	{
		USkeletalMeshComponent* Mesh = Owner->GetMesh();
		if (Mesh)
		{
			USkeletalMesh* SkelMesh = Mesh->SkeletalMesh;

			for (auto x : SkelMesh->MorphTargetIndexMap)
			{
				auto MorphName = x.Key;
				float MorphValue = FMath::FRandRange(-0.5f, 0.9f); // we should limit a bit otherwise we have troglodytes
				Mesh->SetMorphTarget(*MorphName.ToString(), MorphValue);

				FaceDetails.Emplace(MorphName, MorphValue);
			}

		}

	}

}

void UMorphManager::LoadFace(FEnemyCharacterDetails* CharacterDetails)
{
	/* Loads face features of owner character from save file. */
	if (Owner)
	{
		// Get FaceDetails from save file
		auto FaceDetailsToLoad = CharacterDetails->FaceDetails;
		FaceDetails = FaceDetailsToLoad;

		USkeletalMeshComponent* Mesh = Owner->GetMesh();
		if (Mesh)
		{
			for (auto x : FaceDetailsToLoad)
			{
				Mesh->SetMorphTarget(*x.Key.ToString(), x.Value);
			}
		}
	}
}