// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_SetMovementSpeed.h"

#include "AI/NPC/NPCBaseEnemy/NPCBaseEnemy.h"
#include "LGUEDK/Public/AI/NPC/NPCBase/NPCBase.h"
#include "LGUEDK/Public/AI/NPC/NPCBase/NPCBaseController.h"

UBTTask_SetMovementSpeed::UBTTask_SetMovementSpeed(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Set movement speed";
}

EBTNodeResult::Type UBTTask_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}
	
	ANPCBaseEnemy* Enemy = Cast<ANPCBaseEnemy>(AIController->GetPawn());
	if (!Enemy)
	{
		return EBTNodeResult::Failed;
	}
	
	Enemy->SetMovementSpeed(MovementSpeed);

	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
