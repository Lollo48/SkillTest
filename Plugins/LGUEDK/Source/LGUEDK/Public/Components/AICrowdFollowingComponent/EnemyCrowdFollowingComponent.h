// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "EnemyCrowdFollowingComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LGUEDK_API UEnemyCrowdFollowingComponent : public UCrowdFollowingComponent
{
	GENERATED_BODY()

public:

	UEnemyCrowdFollowingComponent();

	int32 GetCrowdAgentAvoidanceGroup() const override;
	int32 GetCrowdAgentGroupsToIgnore() const override;
	int32 GetCrowdAgentGroupsToAvoid() const override;

protected:

	virtual void BeginPlay() override;

};
