// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_StopAttackingTarget.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTService_StopAttackingTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	explicit UBTService_StopAttackingTarget(FObjectInitializer const& ObjectInitializer);
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector AttackTargetKey;
	
	UPROPERTY()
	AActor* CachedPlayerActor;
	UPROPERTY()
	class UHealthBaseComponent* CachedHealthComponent;
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	float TickInterval; 
};
