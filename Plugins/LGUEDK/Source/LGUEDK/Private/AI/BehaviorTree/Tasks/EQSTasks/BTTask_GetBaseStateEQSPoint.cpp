// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_GetBaseStateEQSPoint.h"
#include "AI/EQS/EQSUtility.h"
#include "Algo/RandomShuffle.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/LGDebug.h"

UBTTask_GetBaseStateEQSPoint::UBTTask_GetBaseStateEQSPoint(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Get EQS Point";
	Margin = 0.f;
}

EBTNodeResult::Type UBTTask_GetBaseStateEQSPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::ExecuteTask(OwnerComp, NodeMemory);
	
}

FVector UBTTask_GetBaseStateEQSPoint::PerformTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	InitTask(OwnerComp);

	if (!ControlledPawn || !AttackTarget)
	{
		return FVector::ZeroVector;
	}

	const FVector StartPosition = AttackTarget->GetActorLocation();
	UEQSUtility::RegisterEntity(ControlledPawn, AttackTarget);

	const FVector ControlledPawnPosition = ControlledPawn->GetActorLocation();
	const FVector PlayerPosition = AttackTarget->GetActorLocation();
	const FVector DesiredDirection = (ControlledPawnPosition - PlayerPosition).GetSafeNormal();
	
	auto FindValidPoint = [&](const TArray<FVector>& Points) -> FVector
	{
		for (const FVector& Point : Points)
		{
			const FVector DirectionToPoint = (Point - PlayerPosition).GetSafeNormal();
			const float DotProduct = FVector::DotProduct(DesiredDirection, DirectionToPoint);

			if (DotProduct >= Margin && !IsHittingSomething(StartPosition, Point) && IsPointFree(Point))
			{
				UEQSUtility::RemovePoint(AttackTarget, Point);
				return Point;
			}
		}
		return ControlledPawnPosition;
	};

	if (MinDistanceFromTargetLocation > 0.f)MinDistance = MinDistanceFromTargetLocation;
	if (MaxDistanceFromTargetLocation > 0.f)MaxDistance = MaxDistanceFromTargetLocation;
	
	TArray<FVector> Points = UEQSUtility::TryGetPoint(ControlledPawn, AttackTarget, MinDistance, MaxDistance);
	Algo::RandomShuffle(Points);
	FVector SelectedPoint = FindValidPoint(Points);

	if (!SelectedPoint.IsZero())
	{
		return SelectedPoint;
	}
	
	Points = UEQSUtility::TryGetPoint(ControlledPawn, AttackTarget, MinDistance / 3, MaxDistance / 3);
	Algo::RandomShuffle(Points);
	SelectedPoint = FindValidPoint(Points);

	return SelectedPoint;
}

void UBTTask_GetBaseStateEQSPoint::InitTask(UBehaviorTreeComponent& OwnerComp)
{
	Super::InitTask(OwnerComp);
}



