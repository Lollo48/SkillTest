// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_FlyingActionState.h"

#include "AI/DragonBoss.h"

UBTDecorator_FlyingActionState::UBTDecorator_FlyingActionState(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "FlyingActionState";
}

bool UBTDecorator_FlyingActionState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return false;
	}

	const ADragonBoss* DragonBoss = Cast<ADragonBoss>(AIController->GetPawn());
	if (!DragonBoss)
	{
		return false;
	}

	return DragonBoss->GetFlyingActionState() == RequiredState;
}
