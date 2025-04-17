// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_MovementActionState.h"

#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"

UBTDecorator_MovementActionState::UBTDecorator_MovementActionState(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Movement Action State";
}

bool UBTDecorator_MovementActionState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return false;
	}

	const ANPCBaseStateEnemy* Enemy = Cast<ANPCBaseStateEnemy>(AIController->GetPawn());
	if (!Enemy)
	{
		return false;
	}
	
	return Enemy->GetMovementActionState() == RequiredState;
}
