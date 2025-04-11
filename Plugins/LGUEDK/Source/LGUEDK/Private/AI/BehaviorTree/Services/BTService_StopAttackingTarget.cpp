// Copyright Villains, Inc. All Rights Reserved.


#include "BTService_StopAttackingTarget.h"

#include "AIController.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/HealtComponent/HealthBaseComponent.h"
#include "Utility/LGDebug.h"

UBTService_StopAttackingTarget::UBTService_StopAttackingTarget(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Stop Attacking Target";

	bNotifyBecomeRelevant = true;	
	bNotifyCeaseRelevant = false;	
	
	bNotifyTick = true;           
	Interval = TickInterval; 	
	
	CachedPlayerActor = nullptr;
	CachedHealthComponent = nullptr;
	TickInterval = 1.0f;
	
}

void UBTService_StopAttackingTarget::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();
	if (!CurrentBlackboard)
	{
		LGDebug::Log("Blackboard not initialized", true);
		return;
	}

	CachedPlayerActor = Cast<AActor>(CurrentBlackboard->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	if (CachedPlayerActor)
	{
		CachedHealthComponent = CachedPlayerActor->FindComponentByClass<UHealthBaseComponent>();
		//LGDebug::Log("Player and HealthComponent cached", true);
	}
	
}

void UBTService_StopAttackingTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (CachedPlayerActor && CachedHealthComponent)
	{
		if (CachedHealthComponent->IsDead())
		{
			//LGDebug::Log("Player is dead, stopping attack", true);

			UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();
			if (CurrentBlackboard)
			{
				CurrentBlackboard->SetValueAsObject(AttackTargetKey.SelectedKeyName, nullptr);
			}

			AAIController* AIController = OwnerComp.GetAIOwner();
			ANPCBaseStateEnemyController* BWNPCbaseEnemyController = Cast<ANPCBaseStateEnemyController>(AIController);
			if (BWNPCbaseEnemyController)
			{
				BWNPCbaseEnemyController->SetStateAsPassive();
			}

			CachedPlayerActor = nullptr;
			CachedHealthComponent = nullptr;
		}
	}
}
