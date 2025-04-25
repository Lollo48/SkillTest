// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckDistance.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTService_CheckDistance : public UBTService
{
	GENERATED_BODY()

public:
	
	explicit UBTService_CheckDistance(FObjectInitializer const& ObjectInitializer);
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector ControlledPawnKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector AttackTargetKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MaxDistanceKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MinDistanceKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DistanceFromPlayerKey;

	UPROPERTY()
	UBlackboardComponent* CachedBlackboardComp;

	UPROPERTY()
	UNavigationSystemV1* NavSystem;

	UPROPERTY()
	AActor* AttackTarget;

	UPROPERTY()
	AActor* ControlledPawn;
	
	virtual void OnGreaterDistance(UBehaviorTreeComponent& OwnerComp){}
	virtual void OnLessDistance(UBehaviorTreeComponent& OwnerComp){}
	virtual void OnCantReachPlayer(UBehaviorTreeComponent& OwnerComp){}
	

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	bool bWantChase;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	bool bWantAttack;

	float MaxDistance;
	float MinDistance;
	
};
