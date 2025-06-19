// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_FindRandomLocation.h"

#include "AI/EQS/EQSUtility.h"
#include "AI/NPC/NPCBase/NPCBaseController.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Random Location";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANPCBaseController* const Controller = Cast<ANPCBaseController>(OwnerComp.GetAIOwner());
	if (!Controller){return EBTNodeResult::Failed;}
	
	ANPCBaseStateEnemy* Enemy = Cast<ANPCBaseStateEnemy>(Controller->GetPawn());

	bWantExplore = Cast<ANPCBaseStateEnemy>(Enemy)->GetDataAsset()->bWantExplore;
	FVector InitialSpawnPosition = OwnerComp.GetBlackboardComponent()->GetValueAsVector(InitialPositionKey.SelectedKeyName);
	FVector InitialPosition = Enemy->GetActorLocation();
	float SearchRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(SearchRadiusKey.SelectedKeyName);
	
	FVector BestLocation = UEQSUtility::GetRandomExplorationPoint(Enemy,nullptr,InitialSpawnPosition, InitialPosition, SearchRadius, bWantExplore);
    
	OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", BestLocation);
                
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
