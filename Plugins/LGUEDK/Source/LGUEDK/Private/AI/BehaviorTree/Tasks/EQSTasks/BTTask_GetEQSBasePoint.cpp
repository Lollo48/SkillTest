// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_GetEQSBasePoint.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GetEQSBasePoint::UBTTask_GetEQSBasePoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Get EQS Base Point";
}

EBTNodeResult::Type UBTTask_GetEQSBasePoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FVector TargetPosition = FVector::ZeroVector;

	TargetPosition = PerformTask(OwnerComp,NodeMemory);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetPosition);
	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
	
}

void UBTTask_GetEQSBasePoint::InitTask(UBehaviorTreeComponent& OwnerComp)
{
	AttackTarget = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	ControlledPawn = Cast<ANPCBaseStateEnemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ControlledPawnKey.SelectedKeyName));
	MinDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(MinDistanceKey.SelectedKeyName);
	MaxDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(MaxDistanceKey.SelectedKeyName);
}

FVector UBTTask_GetEQSBasePoint::PerformTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	InitTask(OwnerComp);
	
	if (!AttackTarget) return FVector::ZeroVector;

	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSystem) return AttackTarget->GetActorLocation();

	const float SearchRadius = 250.0f;  
	FNavLocation RandomLocation;

	bool bFound = NavSystem->GetRandomReachablePointInRadius(
		AttackTarget->GetActorLocation(),
		SearchRadius,
		RandomLocation
	);
	
#if !UE_BUILD_SHIPPING
	if (bFound && bShowDebug)
	{
		DrawDebugSphere(GetWorld(), RandomLocation.Location, 20.0f, 12, FColor::Green, false, 2.0f);
	}
#endif

	return bFound ? RandomLocation.Location : AttackTarget->GetActorLocation();
}

bool UBTTask_GetEQSBasePoint::IsHittingSomething(const FVector& Start, const FVector& End) const
{
	FVector StartPosition = Start + FVector(0, 0, 50);
	FVector EndPosition = End + FVector(0, 0, 50);

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(AttackTarget);
	TraceParams.AddIgnoredActor(ControlledPawn);

	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartPosition,
		EndPosition,
		CollisionChannelIsHittingSomething,
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

bool UBTTask_GetEQSBasePoint::IsPointFree(const FVector& Point) const
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ControlledPawn); 
	QueryParams.AddIgnoredActor(AttackTarget);

	FCollisionObjectQueryParams TraceParams;
	TraceParams.AddObjectTypesToQuery(ObjectTypeChannelPointFree);
	
	FHitResult HitResult;
	const bool bHit = GetWorld()->SweepSingleByObjectType(
		HitResult, 
		Point, 
		Point + FVector(0, 0, 10), 
		FQuat::Identity, 
		TraceParams, 
		FCollisionShape::MakeSphere(DistanceBetweenEnemyRadius), 
		QueryParams
	);
	
	const bool bPointIsFree = !bHit;

#if !UE_BUILD_SHIPPING
	if (bShowDebug)
	{
		const FColor SphereColor = bPointIsFree ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), Point, DistanceBetweenEnemyRadius, 12, SphereColor, false, 2.0f);
	}
#endif	
	return bPointIsFree;
}

