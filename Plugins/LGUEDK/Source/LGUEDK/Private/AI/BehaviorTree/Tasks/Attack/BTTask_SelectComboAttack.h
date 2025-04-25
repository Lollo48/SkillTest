// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enumerators/ComboAttack/ComboAttackState.h"
#include "AI/NPC/AttacksData/AttackDataAsset.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SelectComboAttack.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTTask_SelectComboAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	
	explicit UBTTask_SelectComboAttack(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector SelectedAttackKey;
	
	UFUNCTION()
	virtual void GetRightAttackCombo(AActor* InControlledPawn,UBlackboardComponent* Blackboard){};

private:
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector ControlledPawn;
};
