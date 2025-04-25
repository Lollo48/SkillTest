// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DragonSelectionComboAttack.h"

#include "AI/DragonBoss.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTTask_DragonSelectionComboAttack::GetRightAttackCombo(AActor* InControlledPawn,UBlackboardComponent* Blackboard)
{
	ADragonBoss* DragonBoss = Cast<ADragonBoss>(InControlledPawn);
	if (!DragonBoss)return;

	UAttackDataAsset* CurrentAttackCombo = DragonBoss->GetAttackDataAsset();
	if (!CurrentAttackCombo)return;

	Blackboard->SetValueAsEnum(SelectedAttackKey.SelectedKeyName,uint8(CurrentAttackCombo->AttackType));
	
}
