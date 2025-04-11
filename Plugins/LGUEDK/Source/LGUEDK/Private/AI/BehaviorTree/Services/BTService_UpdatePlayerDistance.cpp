// Copyright Villains, Inc. All Rights Reserved.


#include "BTService_UpdatePlayerDistance.h"

#include <string>

#include "AIController.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/LGDebug.h"

UBTService_UpdatePlayerDistance::UBTService_UpdatePlayerDistance(FObjectInitializer const& ObjectInitializer)
{
	NodeName = " Update Player Distance";
	
	ForceInstancing(true);
	
	CachedAttackTarget = nullptr;
	CachedBlackboardComp = nullptr;
	CachedSelf = nullptr;
	
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
}

void UBTService_UpdatePlayerDistance::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (CachedBlackboardComp)
	{
		return; 
	}

	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		LGDebug::Log("Blackboard non trovato", true);
		return;
	}
	
	CachedBlackboardComp = BlackboardComp;
	CachedAttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	
	if (CachedAttackTarget == nullptr)
	{
		LGDebug::Log("AttackTarget non trovato", true);
		return;
	}
	CachedSelf = Cast<AActor>(BlackboardComp->GetValueAsObject(SelfKey.SelectedKeyName));
	if (CachedSelf == nullptr)
	{
		LGDebug::Log("Self non trovato", true);
		return;
	}

	//LGDebug::Log("CachedAttackTarget " + CachedAttackTarget->GetName(), true);
	CachedBlackboardComp->SetValueAsVector(AttackTargetPositionKey.SelectedKeyName, CachedAttackTarget->GetActorLocation());
	CachedBlackboardComp->SetValueAsVector(PawnPositionKey.SelectedKeyName, CachedSelf->GetActorLocation());

}

void UBTService_UpdatePlayerDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!CachedAttackTarget)
	{
		LGDebug::Log("Pawn o AttackTarget non trovati", true);
		return;
	}

	if (!CachedBlackboardComp)
	{
		LGDebug::Log("CachedBlackboardComp non trovati", true);
		return;
	}

	if (!CachedSelf)
	{
		LGDebug::Log("CachedSelf non trovati", true);
		return;
	}
	
	FVector AttackTargetPosition = CachedAttackTarget->GetActorLocation();
	FVector PawnPosition = CachedSelf->GetActorLocation();
	float Distance = FVector::Distance(AttackTargetPosition, PawnPosition);
	
	
	CachedBlackboardComp->SetValueAsVector(AttackTargetPositionKey.SelectedKeyName, AttackTargetPosition);
	CachedBlackboardComp->SetValueAsVector(PawnPositionKey.SelectedKeyName, PawnPosition);
	CachedBlackboardComp->SetValueAsFloat(DistanceFromPlayerKey.SelectedKeyName, Distance);
	
	
	//LGDebug::Log("player position " + CachedAttackTarget->GetActorLocation().ToString(), true);
	//LGDebug::Log("player position " + CachedSelf->GetName(), true);
	
	// LGDebug::Log("enemy position " + CachedSelf->GetActorLocation().ToString(), true);
}


