// Copyright Villains, Inc. All Rights Reserved.


#include "BTDecorator_EnemyState.h"
#include "AIController.h"
#include "AI/NPC/NPCBaseEnemy/NPCBaseEnemy.h"
#include "LGUEDK/Public/AI/NPC/NPCBase/NPCBase.h"

UBTDecorator_EnemyState::UBTDecorator_EnemyState(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Enemy State";
}

bool UBTDecorator_EnemyState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return false;
	}

	const ANPCBaseEnemy* Enemy = Cast<ANPCBaseEnemy>(AIController->GetPawn());
	if (!Enemy)
	{
		return false;
	}
	
	return Enemy->GetState() == RequiredState;
}


