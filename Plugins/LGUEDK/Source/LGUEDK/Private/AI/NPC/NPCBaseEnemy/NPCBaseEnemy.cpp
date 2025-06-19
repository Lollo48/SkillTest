// Copyright Villains, Inc. All Rights Reserved.

#include "LGUEDK/Public/AI/NPC/NPCBaseEnemy/NPCBaseEnemy.h"
#include "AI/Enumerators/EMovementSpeed.h"
#include "AI/NPC/NPCBaseEnemy/NPCPerceptionSystemController.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/LGDebug.h"



ANPCBaseEnemy::ANPCBaseEnemy()
{
	
}

ANPCPerceptionSystemController* ANPCBaseEnemy::GetPerceptionSystemController() const
{
	if (!IsValid(PerceptionSystemController))
	{
		UE_LOG(LogTemp, Error, TEXT("No controller"));
		return nullptr;
	}
	return PerceptionSystemController;
}

void ANPCBaseEnemy::OnEntityPassive()
{
	SetEntityState(EEnemyState::Passive);
	OnStatePassive.Broadcast();
	OnEntityPassiveBP();
}

void ANPCBaseEnemy::OnEntityPatrolling()
{
	//LGDebug::Log("BaseEnemyPatrolling",true);
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
		MyCharacterMovement->MaxWalkSpeed = AIData->IdleSpeed; 
		break;

	case EMovementSpeed::Walk:
		MyCharacterMovement->MaxWalkSpeed = AIData->WalkSpeed; 
		break;

	case EMovementSpeed::Run:
		MyCharacterMovement->MaxWalkSpeed = AIData->RunSpeed; 
		break;

	case EMovementSpeed::Sprint:
		MyCharacterMovement->MaxWalkSpeed = AIData->SprintSpeed; 
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
	PerceptionSystemController = Cast<ANPCPerceptionSystemController>(GetEntityController());
}



