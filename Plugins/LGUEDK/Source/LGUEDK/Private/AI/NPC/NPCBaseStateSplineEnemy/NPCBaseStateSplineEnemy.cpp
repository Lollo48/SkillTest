// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NPC/NPCBaseStateSplineEnemy/NPCBaseStateSplineEnemy.h"

#include "Utility/LGDebug.h"


ANPCBaseStateSplineEnemy::ANPCBaseStateSplineEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(RootComponent);
	CurrentPointIndex = 0;
}


void ANPCBaseStateSplineEnemy::BeginPlay()
{
	Super::BeginPlay();
	// Initialize the spline points
	if (SplineComponent)
	{
		int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
		for (int32 i = 0; i < NumPoints; ++i)
		{
			FVector PointLocation = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
			SplinePoints.Add(PointLocation);
			//LGDebug::Log("Spline Point " + FString::FromInt(i) + ": " + PointLocation.ToString(), true);
			//LGDebug::Log("MaxIndex = " + FString::FromInt(MaxIndex), true);    
		}
		MaxIndex = SplinePoints.Num() - 1;
		CurrentPointIndex = MaxIndex;
		//LGDebug::Log("MaxIndex = " + FString::FromInt(MaxIndex), true);    
	}
}

void ANPCBaseStateSplineEnemy::Tick(float DeltaTime)
{
	CheckPointsDistance();

	if (bDrawDebug)
		DrawDebugSplinePoints();
}
   
FVector ANPCBaseStateSplineEnemy::GetRightPointLocation()
{
	check(SplineComponent != nullptr);
	check(SplinePoints.Num() > 0);     
	if (!SplinePoints.IsValidIndex(CurrentPointIndex))
	{
		LGDebug::Log("Indice spline non valido: " + FString::FromInt(CurrentPointIndex), true);
		return GetActorLocation(); 
	}

	FVector PointLocation = SplinePoints[CurrentPointIndex];

	if (bDrawDebug)
		DrawDebugSphere(GetWorld(), PointLocation, 300.0f, 12, FColor::Red, false, 10.0f, 0, 2.f);

	//LGDebug::Log("GetRightPointLocation: Point before update " + FString::FromInt(CurrentPointIndex), true);

	UpdatePointsIndex();

	//LGDebug::Log("GetRightPointLocation: Point after update  " + FString::FromInt(CurrentPointIndex), true);    
    
	return PointLocation;
}

void ANPCBaseStateSplineEnemy::UpdatePointsIndex()
{
	if (bMovingForward)
	{
		CurrentPointIndex--;
		
		if (CurrentPointIndex == 0)
		{
			if (bPingPong)
				bMovingForward = false;
		}
	}
	else 
	{
		CurrentPointIndex ++;

		if (CurrentPointIndex == MaxIndex)
		{
			if (bPingPong)
				bMovingForward = true;
		}
	}
}

void ANPCBaseStateSplineEnemy::CheckPointsDistance()
{
	const float TolleranzaDistanza = 200.0f;

	if (SplinePoints.Num() == 0) return;

	FVector CurrentLocation = GetActorLocation();
	
	if (FVector::Dist(CurrentLocation, SplinePoints[0]) <= TolleranzaDistanza)
	{
		if (!bWasAtLastPoint)
		{
			if (!bPingPong)
				CurrentPointIndex = -1;
			
			LGDebug::Log("Arrivato all'ULTIMO punto della spline!", true);
			bWasAtFirstPoint = false;
			bWasAtLastPoint = true;
		}
	}
	else if (FVector::Dist(CurrentLocation, SplinePoints[MaxIndex]) <= TolleranzaDistanza)
	{
		if (!bWasAtFirstPoint)
		{
			LGDebug::Log("Arrivato al PRIMO punto della spline!", true);
			bWasAtLastPoint = false;
			bWasAtFirstPoint = true;
		}
	}
}

void ANPCBaseStateSplineEnemy::DrawDebugSplinePoints()
{
	if (!GetWorld() || SplinePoints.Num() == 0) return;

	int index = MaxIndex;

	for (auto Element : SplinePoints)
	{
		DrawDebugSphere(GetWorld(), Element, 255.0f, 12, FColor::Green, false, -1.f, 0, 2.f);
		
		FVector TextLocation = Element + FVector(0, 0, 100); 
		DrawDebugString(GetWorld(), TextLocation, FString::FromInt(index), nullptr, FColor::White, 1.f, false);

		index--;
	}
}


