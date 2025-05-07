// Copyright Villains, Inc. All Rights Reserved.


#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"

#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"



ANPCBaseStateEnemy::ANPCBaseStateEnemy()
{
	Target = nullptr;
	
}

void ANPCBaseStateEnemy::OnEntityPassive()
{
	Super::OnEntityPassive();
}

void ANPCBaseStateEnemy::OnEntityPatrolling()
{
	Super::OnEntityPatrolling();
}

void ANPCBaseStateEnemy::OnEntityInvestigating()
{
	if (!GetIsEnable())return;
	if (!GetIsInitialize())return;
	
	SetEntityState(EEnemyState::Investigating);
	OnStateInvestigating.Broadcast();
	OnEntityInvestigatingBP();
}

void ANPCBaseStateEnemy::OnEntityChasing(AActor* InTarget)
{
	if (!GetIsEnable())return;
	if (!GetIsInitialize())return;
	
	SetTarget(InTarget);
	SetEntityState(EEnemyState::Chasing);
	OnStateChasing.Broadcast(InTarget);
	OnEntityChasingBP(InTarget);
}

void ANPCBaseStateEnemy::OnEntityPending(AActor* InTarget)
{
	if (!GetIsEnable())return;
	if (!GetIsInitialize())return;
	
	SetTarget(InTarget);
	SetEntityState(EEnemyState::Pending);
	OnStatePending.Broadcast(InTarget);
	OnEntityPendingBP(InTarget);
}

void ANPCBaseStateEnemy::OnEntityAttack(AActor* InTarget)
{
	if (!GetIsEnable())return;
	if (!GetIsInitialize())return;
	
	SetTarget(InTarget);
	SetEntityState(EEnemyState::Attacking);
	OnStateAttacking.Broadcast(InTarget);
	OnEntityAttackBP(InTarget);
}

void ANPCBaseStateEnemy::OnEntityDead(AActor* InSelf)
{
	if (!GetIsEnable())return;
	if (!GetIsInitialize())return;
	
	SetEntityState(EEnemyState::Dead);
	OnStateDead.Broadcast(this);
	OnEntityDeadBP(this);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName("Ragdoll", true);
	GetMesh()->SetSimulatePhysics(true);
	
	DisableEntityEffect();

	ANPCBaseStateEnemyController* AIController = Cast<ANPCBaseStateEnemyController>(GetController());
	if (!IsValid(AIController))return;
	AIController->SetStateAsDead(InSelf);
}

UNPCBaseStateEnemyDataAsset* ANPCBaseStateEnemy::GetDataAsset() const
{
	if (!IsValid(NPCEnemyStateDataAsset))
	{
		UE_LOG(LogTemp, Warning, TEXT("NPCEnemyStateDataAsset is nullptr in %s"), *GetName());
		return nullptr;
	}

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


