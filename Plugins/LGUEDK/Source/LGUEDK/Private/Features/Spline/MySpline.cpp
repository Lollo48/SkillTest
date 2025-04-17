// Fill out your copyright notice in the Description page of Project Settings.


#include "Features/Spline/MySpline.h"

#include "VectorTypes.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/LGDebug.h"


// Sets default values
AMySpline::AMySpline()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(RootComponent);
}

void AMySpline::BeginPlay()
{
	Super::BeginPlay();

	if (bDrawDebug)
		DrawDebugSplinePoints();
}

void AMySpline::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!CurrentUser)return;

	if (SplineMode == ESplineMode::SplineModeA)
		CheckPointsDistance();
}

void AMySpline::InitSpline()
{
	if (SplineMode == ESplineMode::SplineModeB)
	{
		if (ACharacter* Character = Cast<ACharacter>(CurrentUser))
		{
			CharacterMovementComponent = Character->GetCharacterMovement();
		}
		SplineLength = SplineComponent->GetSplineLength();
		Direction = 1.0f;
		StopTimer = StopDelay;
		CurrentSplineIndex = 0;
		//LGDebug::Log("SplineLength: " + FString::SanitizeFloat(SplineLength), true);
	}
	else
	{
		CurrentSplinePoints = SplineComponent->GetNumberOfSplinePoints();
		CurrentSplineIndex = 0;
	}

	OnInitSpline.Broadcast();
	InitSplineBP();
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
			CurrentSplineIndex = CurrentSplinePoints - 2;
			bMovingForward = false;
		}
	}
	else
	{
		if (CurrentSplineIndex > 0 )
		{
			CurrentSplineIndex--;
		}
		else if (bPingPong)
		{
			//LGDebug::Log("PingPong: invert to forward", true);
			CurrentSplineIndex = 1;
			bMovingForward = true;
		}
	}
}

void AMySpline::CheckPointsDistance()
{
	//LGDebug::Log("TargetPoint: " + CurrentSplinePointLocation.ToString(), true);
	//LGDebug::Log("CurrentLocation: " + CurrentLocation.ToString(), true);
	//float Distance = FVector::Dist(CurrentSplinePointLocation,CurrentLocation);
	//LGDebug::Log("CurrentLocation: " + FString::FromInt(Distance), true);
	
	FVector CurrentLocation = CurrentUser ->GetActorLocation();
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
			if (bWantsToStartFromBeginning)
				CurrentSplineIndex = -1;
			ReachEndSplinePoint();
			bWasAtLastPoint = true;
		}
		else if (CurrentSplineIndex != CurrentSplinePoints || CurrentSplineIndex != 0)
			ReachSplinePoint();
		
		//LGDebug::Log("ARRIVATO AL PUNTO", true);
		//bIsWaitingAtPoint = true;
		//CurrentWaitTimer = WaitAtPointDuration;
		
		UpdateSplineIndex();
		bIsWaitingAtPoint = false;
	}
}

void AMySpline::FollowSplinePath(float DeltaTime, float Speed)
{
	if (!CharacterMovementComponent || !CurrentUser || !SplineComponent) return;
	
	SplineLength = SplineComponent->GetSplineLength();
	
	if (bStopAtExtremes && StopTimer > 0.0f)
	{
		StopTimer -= DeltaTime;
		
		if (!bMovingForward && StopTimer <= 0.2f)
		{
			FVector VelocityDirection = SplineComponent->GetTangentAtDistanceAlongSpline(DistanceAlongSpline - 1, ESplineCoordinateSpace::World).GetSafeNormal();
			CharacterMovementComponent->Velocity = VelocityDirection * Speed  * Direction;
			FRotator TargetRotation = CharacterMovementComponent->Velocity.Rotation();
			FRotator CurrentRotation = CurrentUser->GetActorRotation();
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, StopDelay);
			CurrentUser->SetActorRotation(NewRotation);
		}
		return;
	}
	
	DistanceAlongSpline += Direction * Speed * DeltaTime;
	DistanceAlongSpline = FMath::Clamp(DistanceAlongSpline, 0.0f, SplineLength);
	
	FVector DesiredLocation = SplineComponent->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	FVector DesiredTangent = SplineComponent->GetTangentAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	DesiredTangent = DesiredTangent.GetSafeNormal() * Direction;
	
	FRotator TargetRotation = DesiredTangent.Rotation();
	TargetRotation.Pitch = 0.0f;
	TargetRotation.Roll = 0.0f;

	FRotator CurrentRotation = CurrentUser->GetActorRotation();
	FRotator SmoothedRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);

	CurrentUser->SetActorLocation(DesiredLocation + FVector(0, 0, OffsetZ));
	CurrentUser->SetActorRotation(SmoothedRotation);
	
	CharacterMovementComponent->Velocity = DesiredTangent * Speed;

	
	if (bMovingForward)
	{
		if (DistanceAlongSpline >= SplineLength)
		{
			ReachEndSplinePoint();

			if (bPingPong)
			{
				Direction = -1.0f;
				bMovingForward = false;
				DistanceAlongSpline = SplineLength;
			}
			if (bStopAtExtremes)
				StopTimer = StopDelay;

			if (bWantsToStartFromBeginning)
				DistanceAlongSpline = 0.0f;
				
		}
	}
	else
	{
		if (DistanceAlongSpline <= 0.0f)
		{
			ReachStartSplinePoint();

			if (bPingPong)
			{
				Direction = 1.0f;
				bMovingForward = true;
				DistanceAlongSpline = 0.0f;
			}

			if (bStopAtExtremes)
				StopTimer = StopDelay;
		}
	}
}

void AMySpline::ReachStartSplinePoint()
{
	//LGDebug::Log("ReachStartSplinePoint",true);
	OnReachStartSplinePoint.Broadcast();
	ReachStartSplinePointBP();
}

void AMySpline::ReachEndSplinePoint()
{
	//LGDebug::Log("ReachEndSplinePoint",true);
	OnReachEndSplinePoint.Broadcast();
	ReachEndSplinePointBP();
}

void AMySpline::ReachSplinePoint()
{
	//LGDebug::Log("ReachSplinePoint",true);
	OnReachSplinePoint.Broadcast();
	ReachSplinePointBP();
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


