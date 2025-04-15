// Copyright Villains, Inc. All Rights Reserved.


#include "LGUEDK/Public/AI/NPC/NPCBase/NPCBase.h"
#include "AI/NPC/NPCBase/NPCBaseController.h"
#include "Components/CapsuleComponent.h"


ANPCBase::ANPCBase()
{
	Controller = nullptr;
	
}

UBehaviorTree* ANPCBase::GetBehaviorTree() const
{
	return BehaviorTree;
}

void ANPCBase::BeginPlay()
{
	Super::BeginPlay();
	Init();
	
}

void ANPCBase::EnableEntity()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController)return;
	if (!AIController->GetBrainComponent())return;
	if (!GetCapsuleComponent())return;
	
	AIController->GetBrainComponent()->StartLogic();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	EnableEntityBP();
}

void ANPCBase::DisableEntity()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController)
	{
		return;
	}

	if (!AIController->GetBrainComponent())return;
	if (!GetCapsuleComponent())return;
	
	AIController->GetBrainComponent()->StopLogic("Dead");
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	DisableEntityBP();
}
