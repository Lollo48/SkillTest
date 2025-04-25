// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LGUEDK/Private/AI/BehaviorTree/Tasks/Attack/BTTask_SelectComboAttack.h"
#include "BTTask_DragonSelectionComboAttack.generated.h"

/**
 * 
 */
UCLASS()
class SKILLTESTPROJECT_API UBTTask_DragonSelectionComboAttack : public UBTTask_SelectComboAttack
{
	GENERATED_BODY()

	virtual void GetRightAttackCombo(AActor* InControlledPawn,UBlackboardComponent* Blackboard) override;
};
