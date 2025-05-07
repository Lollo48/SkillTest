// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_GetBaseTrigger.h"

#include "AIController.h"
#include "TriggerBase/TriggerUtility.h"

UBTTask_GetBaseTrigger::UBTTask_GetBaseTrigger(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Get Base Trigger";
}

EBTNodeResult::Type UBTTask_GetBaseTrigger::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABaseTrigger* CurrentTrigger = GetAvaiableTrigger();

	if (CurrentTrigger == nullptr)return EBTNodeResult::Failed;

	AAIController* const Controller = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (!Controller)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	AActor* ControlledPawn = Controller->GetPawn();
	if (!ControlledPawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}
	
	CurrentTrigger->EnableTrigger(ControlledPawn);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

ABaseTrigger* UBTTask_GetBaseTrigger::GetAvaiableTrigger()
{
	TArray<ABaseTrigger*> AllTriggers = UTriggerUtility::GetTriggerContainer(BaseTriggerTag);
	
	for (auto* Trigger : AllTriggers)
	{
		if (Trigger->IsTriggered())
		{
			return nullptr; 
		}
	}
	
	TArray<ABaseTrigger*> ActivatableTriggers;
	for (auto* Trigger : AllTriggers)
	{
		if (Trigger->CanBeTriggered()) 
		{
			ActivatableTriggers.Add(Trigger);
		}
	}
	
	if (ActivatableTriggers.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, ActivatableTriggers.Num() - 1);
		ABaseTrigger* SelectedTrigger = ActivatableTriggers[RandomIndex];
		return SelectedTrigger;
	}

	return nullptr; 
}
