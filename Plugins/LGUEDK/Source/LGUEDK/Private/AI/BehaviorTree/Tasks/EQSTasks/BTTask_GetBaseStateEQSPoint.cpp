// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_GetBaseStateEQSPoint.h"
#include "AI/EQS/EQSUtility.h"
#include "Algo/RandomShuffle.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/LGDebug.h"

UBTTask_GetBaseStateEQSPoint::UBTTask_GetBaseStateEQSPoint(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Get Attack EQS Point";
	Margin = 0.f;
}

EBTNodeResult::Type UBTTask_GetBaseStateEQSPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::ExecuteTask(OwnerComp, NodeMemory);
	
}

FVector UBTTask_GetBaseStateEQSPoint::PerformTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	if (!ControlledPawn || !AttackTarget)
	{
		return FVector::ZeroVector;
	}

	if (MinDistanceFromTargetLocation > 10.f)MinDistance = MinDistanceFromTargetLocation;
	if (MaxDistanceFromTargetLocation > 10.f)MaxDistance = MaxDistanceFromTargetLocation;

	FGameplayTag AreaTag = ControlledPawn->GetMyAreaTag();
	FVector TargetLocation = AttackTarget->GetActorLocation();
	
	return UEQSUtility::GetAttackAreaPoint(ControlledPawn,AttackTarget,TargetLocation, MinDistance, MaxDistance, Margin, AreaTag);
	
}

void UBTTask_GetBaseStateEQSPoint::InitTask(UBehaviorTreeComponent& OwnerComp)
{
	Super::InitTask(OwnerComp);
}



