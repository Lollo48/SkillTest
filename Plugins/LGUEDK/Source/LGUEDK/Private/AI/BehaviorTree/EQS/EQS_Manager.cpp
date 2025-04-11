// Copyright Villains, Inc. All Rights Reserved.


#include "AI/EQS/EQS_Manager.h"

#include <string>

#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/LGDebug.h"


// Sets default values
AEQS_Manager::AEQS_Manager()
{
	PrimaryActorTick.bCanEverTick = true;
	CanSearchMeleePoint = false;
	Initi();
}

// Called when the game starts or when spawned
void AEQS_Manager::BeginPlay()
{
	Super::BeginPlay();
	CurrentMeleeTime = 0;
	CurrentRangedTime = 0;
	AttackTarget = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	
}

void AEQS_Manager::Tick(float DeltaTime)
{
	if (CanSearchMeleePoint)
	{
		if (CurrentMeleeTime <= 0.0f)
		{
			Points[EEnemyType::Melee].Empty();
			UpdateStrafeMeleePoints();
			CurrentMeleeTime = UpdateMeleeInterval;
			return;
		}
		CurrentMeleeTime -= DeltaTime;
	}

	if (CanSearchRengedPoint)
	{
		if (CurrentRangedTime <= 0.0f)
		{
			Points[EEnemyType::Ranged].Empty();
			UpdateStrafeRangedPoints();
			CurrentRangedTime = UpdateMeleeInterval;
		}
		CurrentRangedTime -= DeltaTime;
	}

	if (CanSearchStrafingPoint)
	{
		if (CurrentStrafingTime <= 0.0f)
		{
			StrafePoints.Empty();
			StrafePoints = SetBaseGridPoints(StrafeGridRadius,StepSizePatrolling);
			CurrentStrafingTime = UpdateStrafingInterval;
		}
		CurrentStrafingTime -= DeltaTime;
	}
}

void AEQS_Manager::Initi()
{
	Points = TMap<EEnemyType,TArray<FVector>>();
	Points.Add(EEnemyType::Melee);
	Points.Add(EEnemyType::Ranged);
}

void AEQS_Manager::UpdateStrafeMeleePoints()
{
	TArray<FVector> MeleePoints;
	
	const float MinRadius = MinAttackRadiusMelee;
	const float MaxRadius = MaxAttackRadiusMelee;
	
	const float GridRadius = MaxRadius;
	const int32 GridSize = FMath::CeilToInt(GridRadius / StepSizeMelee);  

	MeleePoints = SetGridPoints(MeleePoints,GridSize,StepSizeMelee,MinRadius,MaxRadius);

#if !UE_BUILD_SHIPPING
	if (bShowDebug)
	{
		for (const FVector& Point : MeleePoints)
		{
			DrawDebugSphere(
				GetWorld(), 
				Point,				
				50.0f,             
				12,               
				FColor::Red,     
				false,             
				UpdateMeleeInterval            
			);
		}
	}
	
#endif
	
	Points.Add(EEnemyType::Melee, MeleePoints);
}

void AEQS_Manager::UpdateStrafeRangedPoints()
{
	const float MinRadius = MinAttackRadiusRanged;
	const float MaxRadius = MaxAttackRadiusRanged;

	TArray<FVector> RangedPoints;

	const float GridRadius = MaxRadius;
	const int32 GridSize = FMath::CeilToInt(GridRadius / StepSizeRanged); 

	RangedPoints = SetGridPoints(RangedPoints,GridSize,StepSizeRanged,MinRadius,MaxRadius);

#if !UE_BUILD_SHIPPING
	if (bShowDebug)
	{
		for (const FVector& Point : RangedPoints)
		{
			DrawDebugSphere(
				GetWorld(), 
				Point,				
				50.0f,             
				12,                
				FColor::Green,     
				false,             
				UpdateRangedInterval               
			);
		}
	}
#endif
	
	Points.Add(EEnemyType::Ranged, RangedPoints);
}

TArray<FVector> AEQS_Manager::SetGridPoints(TArray<FVector>& PossiblePoints,float const GridSize, float const StepSize,float const MinimusRadius, float const MaximusRadius)
{
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSystem) return PossiblePoints;

	const ANavigationData* NavData = NavSystem->GetDefaultNavDataInstance(FNavigationSystem::ECreateIfEmpty::DontCreate);
	if (!NavData) return PossiblePoints;

	FNavLocation NavLocation;
	
	FVector TargetLocation = AttackTarget->GetActorLocation();
	
	for (int32 X = -GridSize; X <= GridSize; ++X)
	{
		for (int32 Y = -GridSize; Y <= GridSize; ++Y)
		{
			FVector CandidatePoint = TargetLocation + FVector(X * StepSize, Y * StepSize, 0.0f);
			
			if (NavSystem->ProjectPointToNavigation(CandidatePoint, NavLocation, FVector(StepSize, StepSize, 10000.0f)))
			{
				const FPathFindingQuery Query(NavSystem, *NavData, TargetLocation, NavLocation.Location);
				
				if (NavSystem->TestPathSync(Query, EPathFindingMode::Regular))
				{
					if (IsInRange(TargetLocation,NavLocation.Location,MinimusRadius,MaximusRadius) && !IsHittingSomething(TargetLocation,NavLocation.Location))
						PossiblePoints.Add(NavLocation.Location);
				}
			}
		}
	}
	return PossiblePoints;
}

TArray<FVector> AEQS_Manager::SetBaseGridPoints(float const GridRadius,float const StepSize)
{
	TArray<FVector> PossiblePoints;
	
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSystem) return PossiblePoints;

	const ANavigationData* NavData = NavSystem->GetDefaultNavDataInstance(FNavigationSystem::ECreateIfEmpty::DontCreate);
	if (!NavData) return PossiblePoints;
	
	const int32 GridSize = FMath::CeilToInt(GridRadius / StepSize); 
	
	FNavLocation NavLocation;

	FVector TargetLocation = AttackTarget->GetActorLocation();
	
	for (int32 X = -GridSize; X <= GridSize; ++X)
	{
		for (int32 Y = -GridSize; Y <= GridSize; ++Y)
		{
			FVector CandidatePoint = TargetLocation + FVector(X * StepSize, Y * StepSize, 0.0f);
			
			if (NavSystem->ProjectPointToNavigation(CandidatePoint, NavLocation, FVector(StepSize, StepSize, 1000.0f)))
			{
				const FPathFindingQuery Query(NavSystem, *NavData, TargetLocation, NavLocation.Location);
				
				if (NavSystem->TestPathSync(Query, EPathFindingMode::Regular))
				{
					if (IsInRange(TargetLocation,NavLocation.Location,MinStrafeDistance,MaxStrafeDistance))
						PossiblePoints.Add(NavLocation.Location);
				}
			}
		}
	}

#if !UE_BUILD_SHIPPING
	if (bShowDebug)
	{
		for (const FVector& Point : PossiblePoints)
		{
			DrawDebugSphere(
				GetWorld(), 
				Point,				
				50.0f,             
				12,                
				FColor::Green,     
				false,             
				UpdateStrafingInterval           
			);
		}
	}
#endif
	
	return PossiblePoints;
}

bool AEQS_Manager::IsHittingSomething(const FVector& Start, const FVector& End) const
{
	TArray<FHitResult> HitResults;

	const FVector StartPosition = Start + FVector(0,0,50);
	const FVector EndPosition = End + FVector(0, 0, 100);
	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = true;
	TraceParams.AddIgnoredActor(AttackTarget);

	const bool bHit = GetWorld()->LineTraceMultiByChannel(
		HitResults,
		StartPosition,
		EndPosition,
		ECC_Visibility,
		TraceParams
	);

#if !UE_BUILD_SHIPPING
	if (bShowDebug)
	{
		DrawDebugLine(GetWorld(), StartPosition, EndPosition, bHit ? FColor::Red : FColor::Green, false, 2.0f);
	}
#endif
	
	return bHit;
}

bool AEQS_Manager::IsInRange(FVector& TargetPosition,FVector& CurrentPosition,const float& MinDistance, const float& MaxDistance)
{
	const float Distance = FVector::Distance(TargetPosition,CurrentPosition);
	return Distance > MinDistance && Distance < MaxDistance;
}

bool AEQS_Manager::IsPointFree(const FVector& Point, const float Radius, const ANPCBaseStateEnemy* ControlledPawn) const
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ControlledPawn); 
	QueryParams.AddIgnoredActor(AttackTarget); 

	FHitResult HitResult;
	const bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult, 
		Point, 
		Point + FVector(0, 0, 10), 
		FQuat::Identity, 
		CollisionChannel, 
		FCollisionShape::MakeSphere(Radius), 
		QueryParams
	);
	
	const bool bPointIsFree = !bHit;

#if !UE_BUILD_SHIPPING
	if (bShowDebug)
	{
		const FColor SphereColor = bPointIsFree ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), Point, Radius, 12, SphereColor, false, 2.0f);
	}
#endif	
	return bPointIsFree;
}

FVector AEQS_Manager::TryGetPoint(TArray<FVector>& PossiblePoints, const ANPCBaseStateEnemy* RequestActor, float const Margin) const
{
	TArray<FVector> ValidPoints;
	const FVector RequestActorPosition = RequestActor->GetActorLocation();
	const FVector PlayerPosition = AttackTarget->GetActorLocation();
	const FVector DirectionControlledPawnPlayer = (RequestActorPosition - PlayerPosition).GetSafeNormal();

	for (const FVector& Point : PossiblePoints)
	{
		FVector DirectionToPoint = (Point - PlayerPosition).GetSafeNormal();
		const float DotProduct = FVector::DotProduct(DirectionControlledPawnPlayer, DirectionToPoint);

		if (DotProduct >= Margin && IsPointFree(Point, 200.0f, RequestActor))
		{
			ValidPoints.Add(Point);
		}
	}

	if (ValidPoints.Num() > 0)
	{
		const int32 RandomIndex = FMath::RandRange(0, ValidPoints.Num() - 1);
		return ValidPoints[RandomIndex];
	}
	
	return PossiblePoints.Num() > 0 ? PossiblePoints[FMath::RandRange(0, PossiblePoints.Num() - 1)] : FVector::ZeroVector;
}

FVector AEQS_Manager::GetAttackPoint(ANPCBaseStateEnemy* RequestActor,float const Margin)
{
	const EEnemyType EnemyType = RequestActor->GetEnemyType();
	FVector SelectedPoint = FVector::ZeroVector;
	
	TArray<FVector> PossiblePoints;
	
	switch (EnemyType) {
	case EEnemyType::Melee:
		
		if (Points[EEnemyType::Melee].Num() <= 0)
			UpdateStrafeMeleePoints();
		
		PossiblePoints = Points[EEnemyType::Melee];
		CanSearchMeleePoint = true;
		
		break;
	case EEnemyType::Ranged:
		
		if(Points[EEnemyType::Ranged].Num() <= 0)
			UpdateStrafeRangedPoints();
		
		PossiblePoints = Points[EEnemyType::Ranged];
		CanSearchRengedPoint = true;
		
		break;
	}
	
	if (PossiblePoints.Num() > 0)
	{
		SelectedPoint = TryGetPoint(PossiblePoints, RequestActor, Margin);
		Points[EnemyType].Remove(SelectedPoint);
	}

#if !UE_BUILD_SHIPPING
	if (bShowDebug)
	{
		DrawDebugSphere(GetWorld(), SelectedPoint, 30, 12, FColor::Blue, false, 1.0f);
	}
#endif	
	
	return SelectedPoint;
}

FVector AEQS_Manager::GetStrafePoint(ANPCBaseStateEnemy* RequestActor,float const Margin)
{
	FVector SelectedPoint = FVector::ZeroVector;

	CanSearchStrafingPoint = true;
	
	if (StrafePoints.Num() <= 0 )
	{
		StrafePoints = SetBaseGridPoints(StrafeGridRadius,StepSizePatrolling);
	}
	
	SelectedPoint = TryGetPoint(StrafePoints, RequestActor, Margin);
	StrafePoints.Remove(SelectedPoint);	

#if !UE_BUILD_SHIPPING
	if (bShowDebug)
	{
		DrawDebugSphere(GetWorld(), SelectedPoint, 50, 12, FColor::Blue, false, 1.0f);
	}
#endif	
	
	
	return SelectedPoint;
}
