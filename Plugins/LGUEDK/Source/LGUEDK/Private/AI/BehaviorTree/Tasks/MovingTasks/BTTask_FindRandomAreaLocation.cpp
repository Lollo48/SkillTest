// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_FindRandomAreaLocation.h"
#include "NavigationSystem.h"
#include "AI/EQS/EQSUtility.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "LGUEDK/Public/AI/NPC/NPCBase/NPCBaseController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomAreaLocation::UBTTask_FindRandomAreaLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Random Area Location";
}

EBTNodeResult::Type UBTTask_FindRandomAreaLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ANPCBaseController* const Controller = Cast<ANPCBaseController>(OwnerComp.GetAIOwner());
    if (!Controller){return EBTNodeResult::Failed;}
	
	ANPCBaseStateEnemy* Enemy = Cast<ANPCBaseStateEnemy>(Controller->GetPawn());

    bWantExplore = Cast<ANPCBaseStateEnemy>(Enemy)->GetDataAsset()->bWantExplore;
    FVector InitialSpawnPosition = OwnerComp.GetBlackboardComponent()->GetValueAsVector(InitialPositionKey.SelectedKeyName);
    FVector InitialPosition = Enemy->GetActorLocation();
    float SearchRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(SearchRadiusKey.SelectedKeyName);
	FGameplayTag RegionTag = Enemy -> GetMyAreaTag();
	
	FVector BestLocation = UEQSUtility::GetRandomExplorationAreaPoint(Enemy,nullptr,InitialSpawnPosition, InitialPosition, SearchRadius, bWantExplore,RegionTag);
    
    OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", BestLocation);
                
    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}


