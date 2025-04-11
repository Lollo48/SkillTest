// Copyright Villains, Inc. All Rights Reserved.


#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"

#include "AI/Interfaces/AITargetInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/LGDebug.h"


ANPCBaseStateEnemyController::ANPCBaseStateEnemyController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void ANPCBaseStateEnemyController::SetStateAsPassive()
{
	SetCurrentControllerEnemyState(EEnemyState::Passive);
	Super::SetStateAsPassive();
}

void ANPCBaseStateEnemyController::SetStateAsPatrolling()
{
	SetCurrentControllerEnemyState(EEnemyState::Patrolling);
	Super::SetStateAsPatrolling();
}

void ANPCBaseStateEnemyController::SetStateAsInvestigating()
{
	SetCurrentControllerEnemyState(EEnemyState::Investigating);
	SetStateAsInvestigatingBP();
}

void ANPCBaseStateEnemyController::SetStateAsChasing(AActor* InAttackTarget)
{
	SetCurrentControllerEnemyState(EEnemyState::Chasing);
	SetStateAsChasingBP(InAttackTarget);
}

void ANPCBaseStateEnemyController::SetStateAsAttacking(AActor* InAttackTarget)
{
	SetCurrentControllerEnemyState(EEnemyState::Attacking);
	SetStateAsAttackingBP(InAttackTarget);
}

void ANPCBaseStateEnemyController::SetStateAsDead(AActor* InAttackTarget)
{
	SetCurrentControllerEnemyState(EEnemyState::Dead);
	SetStateAsDeadBP(InAttackTarget);
}

void ANPCBaseStateEnemyController::BeginPlay()
{
	Super::BeginPlay();
}

