// Copyright Villains, Inc. All Rights Reserved.

#include "LGUEDK/Public/AI/NPC/NPCBaseEnemy/NPCBaseEnemy.h"
#include "AI/Enumerators/EMovementSpeed.h"
#include "AI/NPC/NPCBase/NPCBaseController.h"
#include "AI/NPC/NPCBaseEnemy/NPCPerceptionSystemController.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/LGDebug.h"



ANPCBaseEnemy::ANPCBaseEnemy()
{
	
}

void ANPCBaseEnemy::OnEntityPassive()
{
	SetEntityState(EEnemyState::Passive);
	OnStatePassive.Broadcast();
	OnEntityPassiveBP();
}

void ANPCBaseEnemy::OnEntityPatrolling()
{
	if (!GetIsEnable())return;
	if (!GetIsInitialize())return;
	
	SetEntityState(EEnemyState::Patrolling);
	OnStatePatrolling.Broadcast();
	OnEntityPatrollingBP();
}

void ANPCBaseEnemy::SetMovementSpeed(const EMovementSpeed MovementSpeed) const
{
	UCharacterMovementComponent* MyCharacterMovement = GetCharacterMovement();
	if (!MyCharacterMovement)
	{
		LGDebug::Error("CharacterMovement component is null!",true);
		return;
	}
	
	switch (MovementSpeed)
	{
	case EMovementSpeed::Idle:
		MyCharacterMovement->MaxWalkSpeed = NPCDataAsset->IdleSpeed; 
		break;

	case EMovementSpeed::Walk:
		MyCharacterMovement->MaxWalkSpeed = NPCDataAsset->WalkSpeed; 
		break;

	case EMovementSpeed::Run:
		MyCharacterMovement->MaxWalkSpeed = NPCDataAsset->RunSpeed; 
		break;

	case EMovementSpeed::Sprint:
		MyCharacterMovement->MaxWalkSpeed = NPCDataAsset->SprintSpeed; 
		break;

	default:
		LGDebug::Error("EMovement speed invalid",true);
		break;
	}

	//LGDebug::Log(*StaticEnum<EMovementSpeed>()->GetNameByValue((int64)MovementSpeed).ToString(),true);
	
}

void ANPCBaseEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ANPCBaseEnemy::Init()
{
	Super::Init();
}



