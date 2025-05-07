// Copyright Villains, Inc. All Rights Reserved.


#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"

#include "AI/Interfaces/AITargetInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/LGDebug.h"


ANPCBaseStateEnemyController::ANPCBaseStateEnemyController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void ANPCBaseStateEnemyController::SetStateAsPassive()
{
	SetCurrentControllerEnemyState(EEnemyState::Passive);
	Super::SetStateAsPassive();

	if (!IsValid(Entity))return;
	Entity->OnEntityPassive();
}

void ANPCBaseStateEnemyController::SetStateAsPatrolling()
{
	SetCurrentControllerEnemyState(EEnemyState::Patrolling);
	Super::SetStateAsPatrolling();

	if (!IsValid(Entity))return;
	Entity->OnEntityPatrolling();
}

void ANPCBaseStateEnemyController::SetStateAsInvestigating()
{
	SetCurrentControllerEnemyState(EEnemyState::Investigating);
	SetStateAsInvestigatingBP();

	if (!IsValid(Entity))return;
	Entity->OnEntityInvestigating();
}

void ANPCBaseStateEnemyController::SetStateAsChasing(AActor* InAttackTarget)
{
	SetCurrentControllerEnemyState(EEnemyState::Chasing);
	SetStateAsChasingBP(InAttackTarget);

	if (!IsValid(Entity))return;
	Entity->OnEntityChasing(InAttackTarget);
}

void ANPCBaseStateEnemyController::SetStateAsAttacking(AActor* InAttackTarget)
{
	SetCurrentControllerEnemyState(EEnemyState::Attacking);
	SetStateAsAttackingBP(InAttackTarget);

	if (!IsValid(Entity))return;
	Entity->OnEntityAttack(InAttackTarget);
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

void ANPCBaseStateEnemyController::InitializeEnemyBase()
{
	Super::InitializeEnemyBase();
	Entity = Cast<ANPCBaseStateEnemy>(GetPawn());
}


