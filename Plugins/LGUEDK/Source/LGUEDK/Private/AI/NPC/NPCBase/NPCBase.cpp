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
	MeshScale = GetMesh()->GetRelativeScale3D();
	ResetEntityCollision();

	BaseController = Cast<ANPCBaseController>(GetController());
	if (!IsValid(BaseController))
	{
		UE_LOG(LogTemp, Error, TEXT("No controller"));
		return;
	}
	
	InitBP();
	SetIsInitialize(true);
}

void ANPCBase::EnableEntity()
{
	EnableEntityBP();
	SetIsEnable(true);
	
	if (!IsValid(BaseController))return;
	BaseController->GetBrainComponent()->StartLogic();
}

void ANPCBase::DisableEntity()
{
	DisableEntityBP();
	SetIsEnable(false);

	if (!IsValid(BaseController))return;
	BaseController->GetBrainComponent()->StopLogic("Event");
}

ANPCBaseController* ANPCBase::GetEntityController() const
{
	if (!IsValid(BaseController))
	{
		UE_LOG(LogTemp, Error, TEXT("No controller"));
		return nullptr;
	}
	return BaseController;
}

void ANPCBase::ResetEntityCollision()
{
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName(EnemyMeshCollisionProfileName);
	GetMesh()->SetCollisionObjectType(CollisionMeshChannel);
	
	GetCapsuleComponent()->SetCollisionProfileName(EnemyCapsuleCollisionProfileName);
	GetCapsuleComponent()->SetCollisionObjectType(CollisionCapsuleChannel);
}

void ANPCBase::ResetEntityTransform()
{
	GetMesh()->SetRelativeLocation(MeshLocation);
	GetMesh()->SetRelativeRotation(MeshRotation);
}
