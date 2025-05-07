// Copyright Villains, Inc. All Rights Reserved.


#include "LGUEDK/Public/AI/NPC/NPCBase/NPCBase.h"
#include "AI/NPC/NPCBase/NPCBaseController.h"
#include "Components/CapsuleComponent.h"


ANPCBase::ANPCBase()
{
	Controller = nullptr;
	BehaviorTree = nullptr;
	bIsInitialize = false;
	bIsEnable = false;
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

void ANPCBase::Init()
{
	MeshLocation = GetMesh()->GetRelativeLocation();
	MeshRotation = GetMesh()->GetRelativeRotation();
	SetEntityCollision();
	InitBP();
	SetIsInitialize(true);
}

void ANPCBase::EnableEntity()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController)return;
	if (!AIController->GetBrainComponent())return;
	
	AIController->GetBrainComponent()->StartLogic();
	EnableEntityBP();
	SetIsEnable(true);
}

void ANPCBase::DisableEntity()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController)return;
	if (!AIController->GetBrainComponent())return;
	
	AIController->GetBrainComponent()->StopLogic("Event");
	DisableEntityBP();
	SetIsEnable(false);
}

void ANPCBase::SetEntityCollision()
{
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName(EnemyMeshCollisionProfileName);
	GetMesh()->SetCollisionObjectType(CollisionMeshChannel);
	
	GetCapsuleComponent()->SetCollisionProfileName(EnemyCapsuleCollisionProfileName);
	GetCapsuleComponent()->SetCollisionObjectType(CollisionCapsuleChannel);
}

void ANPCBase::SetEntityTransform()
{
	GetMesh()->SetRelativeLocation(MeshLocation);
	GetMesh()->SetRelativeRotation(MeshRotation);
}
