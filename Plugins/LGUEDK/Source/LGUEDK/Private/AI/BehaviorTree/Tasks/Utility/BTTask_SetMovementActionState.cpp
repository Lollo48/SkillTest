// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetMovementActionState.h"

#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"

UBTTask_SetMovementActionState::UBTTask_SetMovementActionState(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Set Movement Action State";
}

EBTNodeResult::Type UBTTask_SetMovementActionState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}
	
	ANPCBaseStateEnemy* Entity = Cast<ANPCBaseStateEnemy>(AIController->GetPawn());
	if (!Entity)
	{
		return EBTNodeResult::Failed;
	}
	
	Entity->SetMovementActionState(MovementActionState);

	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
