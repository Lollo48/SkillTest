// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PerformComboAttack.h"

#include "AI/DragonBoss.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_PerformComboAttack::UBTTask_PerformComboAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Perform Combo Attack";
}

EBTNodeResult::Type UBTTask_PerformComboAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	AActor* ControlledActor = Cast<AActor>(Blackboard->GetValueAsObject(ControlledPawn.SelectedKeyName));
	if (!ControlledActor) return EBTNodeResult::Failed;

	ADragonBoss* DragonBoss = Cast<ADragonBoss>(ControlledActor);
	if (!DragonBoss) return EBTNodeResult::Failed;

	DragonBoss->PerformAttackCombo();
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
