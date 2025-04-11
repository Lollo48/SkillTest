// Copyright Villains, Inc. All Rights Reserved.


#include "Features/BezierCurve/BezierCurve.h"
#include "Utility/LGDebug.h"


// Sets default values
ABezierCurve::ABezierCurve()
{
}


void ABezierCurve::BeginPlay()
{
	Super::BeginPlay();
	CalculateStartEndLocations();
}

void ABezierCurve::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	CalculateStartEndLocations();
	DrawCurve();
	AnimateDebugBox(GetWorld()->GetDeltaSeconds());
}


FVector ABezierCurve::CalculateBezierPoint(float Alpha) const
{
	//(1 - Alpha)^2 * StartLocation + 2 * (1-Alpha) * Alpha * MidleLocation + Alpha^2 * EndLocation

	Alpha = FMath::Clamp(Alpha, 0.0f, 1.0f);
	float OneMinusAlpha = 1 - Alpha;
    
	return (OneMinusAlpha * OneMinusAlpha * WorldStartPosition) + (2 * OneMinusAlpha * Alpha * WorldMiddlePosition) + (Alpha * Alpha * WorldEndPosition);
}

void ABezierCurve::DrawCurve() const
{
	
	FVector CurrentPoint = WorldStartPosition;

	for (int i = 1; i <= CurveResolution; i++)
	{
		float Alpha = i / static_cast<float>(CurveResolution);
		
		FVector NewPoint = CalculateBezierPoint(Alpha);
		
		DrawDebugLine(GetWorld(), CurrentPoint, NewPoint, FColor::Blue, false, 0.1f, 0, 0.5f);

		CurrentPoint = NewPoint;
	}
	
	
	//DrawDebugBox(GetWorld(), WorldStartPosition, FVector(30, 30, 30), FColor::Blue, false, 0.1f);
	//DrawDebugBox(GetWorld(), WorldMiddlePosition, FVector(30, 30, 30), FColor::Yellow, false, 0.1f);
	//DrawDebugBox(GetWorld(), WorldEndPosition, FVector(30, 30, 30), FColor::Red, false, 0.1f);
}

void ABezierCurve::AnimateDebugBox(float DeltaTime) 
{
	TimeElapsed += DeltaTime * Speed;
	
	if (TimeElapsed >= 1)
	{
		TimeElapsed = 0;
	}
	
	FVector NewLocation = CalculateBezierPoint(TimeElapsed);
	
	DrawDebugBox(GetWorld(), NewLocation, FVector(30, 30, 30), FColor::Cyan, false, 0.1f);
}

void ABezierCurve::CalculateStartEndLocations()
{
	FTransform ActorTransform = GetActorTransform();
	WorldStartPosition = ActorTransform.TransformPosition(StartLocation);

	//LGDebug::Log(WorldStartPosition.ToString(),true);
	
	FHitResult HitResult;

	 bool bHit = GetWorld()->SweepSingleByChannel(
			HitResult, 
			WorldStartPosition,
			WorldStartPosition,
			FQuat::Identity,
			ECC_Visibility,
			FCollisionShape::MakeSphere(50.0f)
		);
	
	
	if (bHit)
	{
		WorldStartPosition = HitResult.ImpactPoint;
		DrawDebugPoint(GetWorld(),HitResult.ImpactPoint, 20.0f, FColor::Red, false, 0.01f);
	}

	DrawDebugSphere(GetWorld(), WorldStartPosition, 50.0f, 12, bHit ? FColor::Green : FColor::Red, false, 0.01f);
	
	WorldMiddlePosition = ActorTransform.TransformPosition(MiddleLocation);
	
	WorldEndPosition = ActorTransform.TransformPosition(EndLocation);
	
		bHit = GetWorld()->SweepSingleByChannel(
				HitResult, 
				WorldEndPosition,
				WorldEndPosition,
				FQuat::Identity,
				ECC_Visibility,
				FCollisionShape::MakeSphere(50.0f)
			);

	if (bHit)
	{
		WorldEndPosition = HitResult.ImpactPoint;
		DrawDebugPoint(GetWorld(),HitResult.ImpactPoint, 20.0f, FColor::Red, false, 0.01f);
	}

	DrawDebugSphere(GetWorld(), WorldEndPosition, 50.0f, 12, bHit ? FColor::Green : FColor::Red, false, 0.01f);
}


