// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectSnapper.h"
#include "DrawDebugHelpers.h"

// Sets default values
AObjectSnapper::AObjectSnapper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObjectSnapper::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObjectSnapper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


float AObjectSnapper::CalculateHeight(AUsableActor* Actor)
{
	return 0.f;
}

void AObjectSnapper::SnapToGround(AUsableActor* Actor, UWorld* World, FVector Location)
{
	// Z to change
	float TotalZ = 0.f;

	// Get item location
	FVector ActorLocation = Actor->GetActorLocation();

	// Get item height(size)
	auto ItemHeightSize = Actor->GetComponentsBoundingBox().GetSize().Z;

	// Create Line Trace
	float TraceLength = 1000.f;

	// Set initial start and end positions of line
	float LineStartOffsetZ = 200.f;
	FVector Start = FVector(Location.X, Location.Y, ActorLocation.Z + LineStartOffsetZ);
	FVector End = Start;

	FCollisionQueryParams CollisionParams(FName(TEXT("LandscapeTracer")), true, this);
	float DistanceToFloor = 0;
	int iteration = 0;
	FString HittedObjectName = "";
	FHitResult OutHit(ForceInit);
	bool Success = true;
	while (TraceLength > 1.f || TraceLength < -1.f)
	{
		HittedObjectName = "";

		End.Z -= TraceLength;

		bool isHit = World->LineTraceSingleByChannel(OutHit, Start, End, ECC_WorldStatic, CollisionParams);

		DrawDebugLine(World, Start, End, FColor(255.f,125.f,125.f), true, 5.0f);

		auto HittedActor = OutHit.GetActor();
		if (HittedActor)
		{
			auto HittedClass = HittedActor->GetClass();
			if (HittedClass)
			{
				HittedObjectName = HittedClass->GetName();
				UE_LOG(LogTemp, Warning, TEXT("HittedActor: %s"), *HittedObjectName);
			}
		}

		// if linetrace hit the landscape
		if (HittedObjectName == "StaticMeshActor")
		{
			// Snap reverse almost never gets called. Lots of code duplication too. But just here to prevent bugs.
			SnapReverse(Actor, World, Location);
			return;
		}

		// if linetrace hit the landscape
		if (HittedObjectName == "Landscape")
		{
			End.Z += TraceLength;
			TraceLength /= 4;
			//UE_LOG(LogTemp, Warning, TEXT("TraceLength: %f"), TraceLength);
		}

		// if linetrace did not hit the landscape
		else if (HittedObjectName == "")
		{
			Start = End;
			DistanceToFloor += TraceLength;
		}
		// if linetrace hit object but not landscape
		else
		{
			End.Z += TraceLength;
			TraceLength /= 4;
		}

		// "end of current loop" operations
		iteration++;
		if (iteration > 25)
		{
			// Snap reverse almost never gets called. Lots of code duplication too. But just here to prevent bugs.
			SnapReverse(Actor, World, Location);
			return;
		}
	}

	//if(Success)
	Actor->SetActorLocation(FVector(Location.X, Location.Y, ActorLocation.Z - DistanceToFloor + LineStartOffsetZ));

}

void AObjectSnapper::SnapReverse(AUsableActor* Actor, UWorld* World, FVector Location)
{
	/* if object is going underground snap it back to surface*/
		// Z to change
	float TotalZ = 0.f;

	// Get item location
	FVector ActorLocation = Actor->GetActorLocation();
	//Actor->MeshComp->

	// Get item height(size)
	auto ItemHeightSize = Actor->GetComponentsBoundingBox().GetSize().Z;

	// Create Line Trace
	float TraceLength = -1000.f;

	// Set initial start and end positions of line
	FVector Start = FVector(Location.X, Location.Y, ActorLocation.Z + ItemHeightSize);
	FVector End = Start;

	FCollisionQueryParams CollisionParams(FName(TEXT("LandscapeTracer")), true, this);
	float DistanceToFloor = 0;
	int iteration = 0;
	FString HittedObjectName = "";
	FHitResult OutHit(ForceInit);
	bool Success = true;
	while (TraceLength > 5.f || TraceLength < -5.f)
	{
		HittedObjectName = "";

		End.Z -= TraceLength;

		bool isHit = World->LineTraceSingleByChannel(OutHit, Start, End, ECC_WorldStatic, CollisionParams);

		DrawDebugLine(World, Start, End, FColor(255.f, 125.f, 125.f), true, 5.0f);



		auto HittedActor = OutHit.GetActor();
		if (HittedActor)
		{
			auto HittedClass = HittedActor->GetClass();
			if (HittedClass)
			{
				HittedObjectName = HittedClass->GetName();
			}
		}

		// if linetrace hit the landscape
		if (HittedObjectName == "StaticMeshActor")
		{
			//Success = false;
			return;
		}

		// if linetrace hit the landscape
		if (HittedObjectName == "Landscape")
		{
			End.Z += TraceLength;
			TraceLength /= 2;
			//UE_LOG(LogTemp, Warning, TEXT("TraceLength: %f"), TraceLength);
		}

		// if linetrace did not hit the landscape
		else if (HittedObjectName == "")
		{
			Start = End;
			DistanceToFloor += TraceLength;
		}
		// if linetrace hit object but not landscape
		else
		{
			End.Z += TraceLength;
			TraceLength /= 2;
		}

		// "end of current loop" operations
		iteration++;
		if (iteration > 25)
		{
			Success = false;
			return;
		}
	}
	//if(Success)
	Actor->SetActorLocation(FVector(Location.X, Location.Y, ActorLocation.Z - DistanceToFloor + ItemHeightSize *1.05f/*+ (ItemHeightSize/2)*/));
}