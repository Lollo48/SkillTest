// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_RotateEntity.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTService_RotateEntity : public UBTService
{
	GENERATED_BODY()

public:
	
	explicit UBTService_RotateEntity(FObjectInitializer const& ObjectInitializer);
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector ControlledPawnKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector AttackTargetKey;

	UPROPERTY()
	AActor* AttackTarget;

	UPROPERTY()
	AActor* ControlledPawn;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float RotationSpeed;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float MaxYaw = 80.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float MinYaw = -80.0f;
	
};
