// Copyright Villains, Inc. All Rights Reserved.


#include "Components/AICrowdFollowingComponent/EnemyCrowdFollowingComponent.h"

#include "AI/RVOAvoidanceInterface.h"


UEnemyCrowdFollowingComponent::UEnemyCrowdFollowingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

int32 UEnemyCrowdFollowingComponent::GetCrowdAgentAvoidanceGroup() const
{
	return 1;//AvoidanceInterface.IsValid() ? AvoidanceInterface->GetAvoidanceGroupMask() : 0;
}

int32 UEnemyCrowdFollowingComponent::GetCrowdAgentGroupsToIgnore() const
{
	return 1;//AvoidanceInterface.IsValid() ? AvoidanceInterface->GetGroupsToIgnoreMask() : 0;
}

int32 UEnemyCrowdFollowingComponent::GetCrowdAgentGroupsToAvoid() const
{
	return 1;//AvoidanceInterface.IsValid() ? AvoidanceInterface->GetGroupsToAvoidMask() : 0;
}


void UEnemyCrowdFollowingComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


