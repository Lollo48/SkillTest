// Copyright Villains, Inc. All Rights Reserved.


#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"

#include "AI/Interfaces/AITargetInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/LGDebug.h"


ANPCBaseStateEnemyController::ANPCBaseStateEnemyController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

ANPCBaseStateEnemy* ANPCBaseStateEnemyController::GetBaseStateControlledEntity() const
{
	if (!IsValid(BaseStateControlledEntity))
	{
		UE_LOG(LogTemp, Error, TEXT("No controlled pawn"));
		return nullptr;
	}
	return BaseStateControlledEntity;
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

	if (!IsValid(BaseStateControlledEntity))return;
	BaseStateControlledEntity->OnEntityInvestigating();
}

void ANPCBaseStateEnemyController::SetStateAsChasing(AActor* InAttackTarget)
{
	SetCurrentControllerEnemyState(EEnemyState::Chasing);
	SetStateAsChasingBP(InAttackTarget);

	if (!IsValid(BaseStateControlledEntity))return;
	BaseStateControlledEntity->OnEntityChasing(InAttackTarget);
}

void ANPCBaseStateEnemyController::SetStateAsAttacking(AActor* InAttackTarget)
{
	SetCurrentControllerEnemyState(EEnemyState::Attacking);
	SetStateAsAttackingBP(InAttackTarget);

	if (!IsValid(BaseStateControlledEntity))return;
	BaseStateControlledEntity->OnEntityAttack(InAttackTarget);
}

void ANPCBaseStateEnemyController::SetStateAsPending(AActor* InAttackTarget)
{
	SetCurrentControllerEnemyState(EEnemyState::Pending);
	SetStateAsPendingBP(InAttackTarget);

	if (!IsValid(BaseStateControlledEntity))return;
	BaseStateControlledEntity->OnEntityPending(InAttackTarget);
}

void ANPCBaseStateEnemyController::SetStateAsDead(AActor* InAttackTarget)
{
	SetCurrentControllerEnemyState(EEnemyState::Dead);
	SetStateAsDeadBP(InAttackTarget);

	if (!IsValid(BaseStateControlledEntity))return;
	BaseStateControlledEntity->OnEntityDead(InAttackTarget);
}

void ANPCBaseStateEnemyController::BeginPlay()
{
	Super::BeginPlay();
}

void ANPCBaseStateEnemyController::InitializeControlledEntity()
{
	BaseStateControlledEntity = Cast<ANPCBaseStateEnemy>(GetControlledEntity());
}


