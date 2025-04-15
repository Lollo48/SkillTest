// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Manager/Spline/MySpline.h"

#include "Utility/LGDebug.h"


// Sets default values
AMySpline::AMySpline()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(RootComponent);
}

void AMySpline::InitSpline()
{
	CurrentSplinePoints = SplineComponent->GetNumberOfSplinePoints();
	CurrentSplineIndex = 0;
	
	OnInitSpline.Broadcast();
	InitSplineBP();
}

void AMySpline::ReachStartSplinePoint()
{
	LGDebug::Log("ReachStartSplinePoint",true);
	OnReachStartSplinePoint.Broadcast();
	ReachStartSplinePointBP();
}

void AMySpline::ReachEndSplinePoint()
{
	LGDebug::Log("ReachEndSplinePoint",true);
	OnReachEndSplinePoint.Broadcast();
	ReachEndSplinePointBP();
}

void AMySpline::ReachSplinePoint()
{
	LGDebug::Log("ReachSplinePoint",true);
	OnReachSplinePoint.Broadcast();
	ReachSplinePointBP();
}

FVector AMySpline::GetRightPointAtSplineIndex()
{
	CurrentSplinePointLocation = SplineComponent->GetLocationAtSplinePoint(CurrentSplineIndex, ESplineCoordinateSpace::World);
	bIsWaitingAtPoint = true;
	if (bDrawDebug)
	{
		DrawDebugSphere(GetWorld(), CurrentSplinePointLocation, 300.0f, 12, FColor::Red, false, 10.0f, 0, 2.f);
	}
	
	//LGDebug::Log("PointLocation: " + CurrentSplinePointLocation.ToString(),true);
	//LGDebug::Log("CurrentSplinePoints: " + FString::FromInt(CurrentSplinePoints), true);
	//LGDebug::Log("CurrentSplineIndex: " + FString::FromInt(CurrentSplineIndex), true);
	
	return CurrentSplinePointLocation;
}

void AMySpline::FollowSplinePath(float DeltaTime, float Speed)
{
	const float MoveStep = Speed * DeltaTime;
	

}

void AMySpline::BeginPlay()
{
	Super::BeginPlay();
	InitSpline();

	if (bDrawDebug)
		DrawDebugSplinePoints();
}

void AMySpline::UpdateSplineIndex()
{
	if (bMovingForward)
	{
		if (CurrentSplineIndex < CurrentSplinePoints - 1)
		{
			CurrentSplineIndex++;
		}
		else if (bPingPong)
		{
			//LGDebug::Log("PingPong: invert to backward", true);
			bMovingForward = false;
		}
	}
	else
	{
		if (CurrentSplineIndex > 0)
		{
			CurrentSplineIndex--;
		}
		else if (bPingPong)
		{
			//LGDebug::Log("PingPong: invert to forward", true);
			bMovingForward = true;
		}
	}
}

void AMySpline::CheckPointsDistance()
{
	FVector CurrentLocation = CurrentUser ->GetActorLocation();
	//LGDebug::Log("TargetPoint: " + CurrentSplinePointLocation.ToString(), true);
	//LGDebug::Log("CurrentLocation: " + CurrentLocation.ToString(), true);

	//float Distance = FVector::Dist(TargetPoint,CurrentLocation);
	//LGDebug::Log("CurrentLocation: " + FString::FromInt(Distance), true);

	if (FVector::Dist(CurrentLocation, CurrentSplinePointLocation) <= Tolerance && bIsWaitingAtPoint)
	{
		if (CurrentSplineIndex == 0 && !bWasAtFirstPoint)
		{
			if (bWantsContinueEvent)
			{
				bWasAtLastPoint = false;
			}
			ReachStartSplinePoint();
			bWasAtFirstPoint = true;
		}
		else if (CurrentSplineIndex == CurrentSplinePoints - 1 && !bWasAtLastPoint)
		{
			if (bWantsContinueEvent)
			{
				bWasAtFirstPoint = false;
			}
			ReachEndSplinePoint();
			bWasAtLastPoint = true;
		}
		else
			ReachSplinePoint();
		//LGDebug::Log("ARRIVATO AL PUNTO", true);
		//bIsWaitingAtPoint = true;
		//CurrentWaitTimer = WaitAtPointDuration;
		
		UpdateSplineIndex();
		bIsWaitingAtPoint = false;
	}
}

void AMySpline::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!CurrentUser)return;
	
	CheckPointsDistance();
}

void AMySpline::DrawDebugSplinePoints()    
{
	if (!GetWorld()) return;

	int32 Index = 0;

	const int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
	
	for (int32 i = 0; i < NumPoints; ++i)
	{
		FVector CurrentPoint = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);

		DrawDebugSphere(GetWorld(), CurrentPoint, 255.0f, 12, FColor::Green, false, 1000.f, 0, 2.f);
		DrawDebugString(GetWorld(), CurrentPoint + FVector(0, 0, 100), FString::FromInt(Index++), nullptr, FColor::White, 1000.f, false);
	}
	
}

