// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SelectComboAttack.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SelectComboAttack::UBTTask_SelectComboAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Select Combo Attack";
}

EBTNodeResult::Type UBTTask_SelectComboAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	AActor* ControlledActor = Cast<AActor>(Blackboard->GetValueAsObject(ControlledPawn.SelectedKeyName));
	if (!ControlledActor) return EBTNodeResult::Failed;

	GetRightAttackCombo(ControlledActor,Blackboard);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
   
}


