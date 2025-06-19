// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_GetChasingPoint.h"

#include "AI/EQS/EQSUtility.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GetChasingPoint::UBTTask_GetChasingPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Get Chasing Point";
}

EBTNodeResult::Type UBTTask_GetChasingPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	AActor* ControlledPawn = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ControlledPawnKey.SelectedKeyName));
	
	float MinDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(MinDistanceKey.SelectedKeyName);
	float MaxDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(MaxDistanceKey.SelectedKeyName);

	FVector TargetLocation = Target->GetActorLocation();

	FVector TargetPosition = UEQSUtility::GetChasingAreaPoint(ControlledPawn,Target,TargetLocation, MinDistance, MaxDistance);

	if (TargetPosition == TargetLocation)
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetPosition);
	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
