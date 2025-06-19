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
	
	InitTask(OwnerComp);
	
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
