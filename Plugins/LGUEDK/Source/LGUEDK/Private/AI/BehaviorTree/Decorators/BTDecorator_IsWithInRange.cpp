// Copyright Villains, Inc. All Rights Reserved.


#include "BTDecorator_IsWithInRange.h"

#include <string>

#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/LGDebug.h"

UBTDecorator_IsWithInRange::UBTDecorator_IsWithInRange(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Is With In Range";

	bAllowAbortNone = true;
	bAllowAbortLowerPri = true;
	bAllowAbortChildNodes = true;
}

bool UBTDecorator_IsWithInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}

	const float MaxRange = BlackboardComp->GetValueAsFloat(MaxAttackRadiusKey.SelectedKeyName);
	const float MinRange = BlackboardComp->GetValueAsFloat(MinAttackRadiusKey.SelectedKeyName);
	
	float DistanceFromPlayer = BlackboardComp->GetValueAsFloat(DistanceFromPlayerKey.SelectedKeyName);

	if (DistanceFromPlayer <= 0)
	{
		return true;
	}
	
	//LGDebug::Log("DistanceFromPlayer: " + FString::SanitizeFloat(DistanceFromPlayer), true);
	// LGDebug::Log("MaxRange: " + FString::SanitizeFloat(MaxRange), true);
	// LGDebug::Log("MinRange: " + FString::SanitizeFloat(MinRange), true);
	
	if (DistanceFromPlayer > MaxRange)
	{
		return true; 
	}
	
	return false;
}
