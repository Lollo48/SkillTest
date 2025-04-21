// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_SetBool.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetBool::UBTTask_SetBool(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Set bool";
}

EBTNodeResult::Type UBTTask_SetBool::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BoolKey.SelectedKeyName,bNewValue);
	
	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
