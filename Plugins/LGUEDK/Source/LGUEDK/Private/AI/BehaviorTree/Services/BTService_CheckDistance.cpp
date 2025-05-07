// Copyright Villains, Inc. All Rights Reserved.


#include "BTService_CheckDistance.h"

#include "NavigationPath.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/LGDebug.h"

UBTService_CheckDistance::UBTService_CheckDistance(FObjectInitializer const& ObjectInitializer)
{
	NodeName = " Check Player Distance";
	
	ForceInstancing(true);
	
	CachedBlackboardComp = nullptr;
	
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
}

void UBTService_CheckDistance::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem)
	{
		return;
	}

	
	CachedBlackboardComp = BlackboardComp;

	MaxDistance = CachedBlackboardComp->GetValueAsFloat(MaxDistanceKey.SelectedKeyName);
	MinDistance = CachedBlackboardComp->GetValueAsFloat(MinDistanceKey.SelectedKeyName);
	AttackTarget = Cast<AActor>(CachedBlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	ControlledPawn = Cast<AActor>(CachedBlackboardComp->GetValueAsObject(ControlledPawnKey.SelectedKeyName));
}

void UBTService_CheckDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!AttackTarget)
	{
		AttackTarget = Cast<AActor>(CachedBlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	}

	if (!NavSystem)
	{
		LGDebug::Warning("NavSystem INVALID ", true);
		return;
	}
	
	UNavigationPath* NavPath = NavSystem->FindPathToLocationSynchronously(GetWorld(), ControlledPawn->GetActorLocation(), AttackTarget->GetActorLocation());

	if (!NavPath)
	{
		LGDebug::Warning("NavPath INVALID ", true);
		return;
	}
	
	if (NavPath->IsPartial())
	{
		OnCantReachPlayer(OwnerComp);
		//LGDebug::Log("CantReachPlayer",true);
		return;
	}
	
	float Distance = CachedBlackboardComp->GetValueAsFloat(DistanceFromPlayerKey.SelectedKeyName);
	
	if (Distance > MaxDistance && bWantChase)
	{
		OnGreaterDistance(OwnerComp);
		return;
	}
	
	if (Distance < MinDistance && bWantAttack)
	{
		OnLessDistance(OwnerComp);
	}
	
}

