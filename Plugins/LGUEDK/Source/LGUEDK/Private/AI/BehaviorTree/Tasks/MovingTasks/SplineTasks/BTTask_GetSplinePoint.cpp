﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetSplinePoint.h"

#include "AI/NPC/NPCBaseStateSplineEnemy/NPCBaseStateSplineEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/LGDebug.h"

UBTTask_GetSplinePoint::UBTTask_GetSplinePoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Get Spline Point";
}

EBTNodeResult::Type UBTTask_GetSplinePoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FVector TargetPosition = FVector::ZeroVector;

	ANPCBaseStateSplineEnemy* ControlledPawn = Cast<ANPCBaseStateSplineEnemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ControlledPawnKey.SelectedKeyName));

	AMySpline* MySpline = ControlledPawn->GetMySpline();
	if (!MySpline)
	{
		LGDebug::Error("Spline not found", true);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}
	
	TargetPosition = MySpline->GetRightPointAtSplineIndex();
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetPosition);
	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
     