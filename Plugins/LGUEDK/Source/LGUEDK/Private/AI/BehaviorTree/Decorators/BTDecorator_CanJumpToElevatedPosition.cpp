// Copyright Villains, Inc. All Rights Reserved.


#include "BTDecorator_CanJumpToElevatedPosition.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/LGDebug.h"

UBTDecorator_CanJumpToElevatedPosition::UBTDecorator_CanJumpToElevatedPosition(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Can Jump to Elevated Position";
	
	bAllowAbortNone = true;
	bAllowAbortLowerPri = true;
	bAllowAbortChildNodes = true;
}

bool UBTDecorator_CanJumpToElevatedPosition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory) const
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}
	
	AActor* AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	UCharacterMovementComponent* MovementComponent = AttackTarget->FindComponentByClass<UCharacterMovementComponent>();
	
	const FVector SelfPosition = BlackboardComp->GetValueAsVector(SelfPositionKey.SelectedKeyName);
	const FVector TargetPosition = BlackboardComp->GetValueAsVector(AttackTargetPositionKey.SelectedKeyName);
	
	if (!FMath::IsNearlyEqual(TargetPosition.Z , SelfPosition.Z ,150))return false;
	
	const float ZDifference = TargetPosition.Z - SelfPosition.Z;
	const float HorizontalDistance = FVector::Distance(TargetPosition,SelfPosition);

	const float MaxJumpHeight = BlackboardComp->GetValueAsFloat(MaxJumpHeightKey.SelectedKeyName);
	const float AttackRange = BlackboardComp->GetValueAsFloat(AttackRangeKey.SelectedKeyName);
	const float MinJumpDistance = BlackboardComp->GetValueAsFloat(MinJumpDistanceKey.SelectedKeyName);
	
	if (ZDifference > MinJumpDistance && ZDifference <= MaxJumpHeight && HorizontalDistance > AttackRange && !MovementComponent->IsFalling() )
	{
		return true;
	}
	
	return false;

	// LGDebug::Log("SelfZPosition " + FString::SanitizeFloat(SelfPosition), true);
	// LGDebug::Log("TargetZPosition " + FString::SanitizeFloat(TargetZPosition), true);
}

