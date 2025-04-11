// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdatePlayerDistance.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTService_UpdatePlayerDistance : public UBTService
{
	GENERATED_BODY()
public:
	
	explicit UBTService_UpdatePlayerDistance(FObjectInitializer const& ObjectInitializer);
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector SelfKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector AttackTargetKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector AttackTargetPositionKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector PawnPositionKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DistanceFromPlayerKey;
	
	UPROPERTY()
	AActor* CachedAttackTarget;
	UPROPERTY()
	AActor* CachedSelf;
	UPROPERTY()
	UBlackboardComponent* CachedBlackboardComp;

	
};
