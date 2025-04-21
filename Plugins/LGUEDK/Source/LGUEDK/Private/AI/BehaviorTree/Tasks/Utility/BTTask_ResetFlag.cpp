// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ResetFlag.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ResetFlag::UBTTask_ResetFlag(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "ResetFlag";
}

EBTNodeResult::Type UBTTask_ResetFlag::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BoolKey.SelectedKeyName,bNewValue);
	
	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
