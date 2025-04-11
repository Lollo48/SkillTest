// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_SetState.h"

#include "AI/NPC/NPCBaseEnemy/NPCBaseEnemy.h"
#include "AI/NPC/NPCBaseEnemy/NPCPerceptionSystemController.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/LGDebug.h"

UBTTask_SetState::UBTTask_SetState(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Set state";
}

EBTNodeResult::Type UBTTask_SetState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	ANPCBaseStateEnemyController* AIController = Cast<ANPCBaseStateEnemyController>(OwnerComp.GetAIOwner());

	if (!AIController)
	{
		LGDebug::Log("non inizializzo il controller",true);
		return EBTNodeResult::Failed;
	}
	
	AIController->SetInitialState(DesiredState);
	
	//LGDebug::Log(*StaticEnum<EEnemyState>()->GetNameByValue((int64)DesiredState).ToString(),true);

	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
