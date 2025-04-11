// Copyright Villains, Inc. All Rights Reserved.


#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"

#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"
#include "Kismet/GameplayStatics.h"



ANPCBaseStateEnemy::ANPCBaseStateEnemy()
{
	AttackTarget = nullptr;
	
}

void ANPCBaseStateEnemy::OnEnemyPassive()
{
	Super::OnEnemyPassive();
}

void ANPCBaseStateEnemy::OnEnemyPatrolling()
{
	Super::OnEnemyPatrolling();
}

void ANPCBaseStateEnemy::OnEnemyInvestigating()
{
	SetEnemyState(EEnemyState::Investigating);
	OnStateInvestigating.Broadcast();
	OnEnemyInvestigatingBP();
}

void ANPCBaseStateEnemy::OnEnemyChasing(AActor* InAttackTarget)
{
	SetAttackTarget(InAttackTarget);
	SetEnemyState(EEnemyState::Chasing);
	OnStateChasing.Broadcast(InAttackTarget);
	OnEnemyChasingBP(InAttackTarget);
}

void ANPCBaseStateEnemy::OnEnemyPending(AActor* InAttackTarget)
{
	SetAttackTarget(InAttackTarget);
	SetEnemyState(EEnemyState::Pending);
	OnStatePending.Broadcast(InAttackTarget);
	OnEnemyPendingBP(InAttackTarget);
}

void ANPCBaseStateEnemy::OnEnemyAttack(AActor* InAttackTarget)
{
	SetAttackTarget(InAttackTarget);
	SetEnemyState(EEnemyState::Attacking);
	OnStateAttacking.Broadcast(InAttackTarget);
	OnEnemyAttackBP(InAttackTarget);
}

void ANPCBaseStateEnemy::OnEnemyDead(AActor* InAttackTarget)
{
	SetEnemyState(EEnemyState::Dead);
	OnStateDead.Broadcast(this);
	OnEnemyDeadBP(this);
	DisableEntity();
	DisableEntityEffectBP();
}

UNPCBaseStateEnemyDataAsset* ANPCBaseStateEnemy::GetDataAsset()
{
	if (!NPCEnemyStateDataAsset) return nullptr;
	return NPCEnemyStateDataAsset;
}

void ANPCBaseStateEnemy::BeginPlay()
{
	Super::BeginPlay();
	NPCEnemyStateDataAsset = Cast<UNPCBaseStateEnemyDataAsset>(NPCDataAsset);
}

void ANPCBaseStateEnemy::Init()
{
	Super::Init();
}


