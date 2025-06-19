// Copyright Villains, Inc. All Rights Reserved.


#include "AI/Manager/FlowGameSystem/Utility/FlowAttacksUtility.h"

UFlowAttacksManager* UFlowAttacksUtility::FlowGameWorldSubSystem = nullptr;

void UFlowAttacksUtility::Init(UFlowAttacksManager* InFlowGameWorldSubSystem)
{
	if (!IsValid(InFlowGameWorldSubSystem))
	{
		//LGDebug::Error("FlowGameWorldSubSystem is not valid", true);
		return;
	}
	
	FlowGameWorldSubSystem = InFlowGameWorldSubSystem;
}

void UFlowAttacksUtility::RegisterEntity(ANPCBaseStateEnemy* Requester, const EEnemyType EnemyType, const FGameplayTag MyTeam)
{
	FlowGameWorldSubSystem -> RegisterEntity(Requester, EnemyType, MyTeam);
}

void UFlowAttacksUtility::UnregisterEntity(ANPCBaseStateEnemy* Requester, const EEnemyType EnemyType, const FGameplayTag MyTeam)
{
	FlowGameWorldSubSystem -> UnregisterEntity(Requester, EnemyType, MyTeam);
}

void UFlowAttacksUtility::UpdateActiveEntity(const EEnemyType EnemyType, const FGameplayTag MyTeam)
{
	FlowGameWorldSubSystem -> UpdateActiveEntity(EnemyType, MyTeam);
}

void UFlowAttacksUtility::ForceRegisterEntity(ANPCBaseStateEnemy* Requester, const EEnemyType EnemyType, const FGameplayTag MyTeam,AActor* Target)
{
	FlowGameWorldSubSystem -> ForceRegisterEntity(Requester, EnemyType, MyTeam,Target);
}
