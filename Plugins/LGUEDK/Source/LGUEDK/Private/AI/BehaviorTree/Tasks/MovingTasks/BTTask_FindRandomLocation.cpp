// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_FindRandomLocation.h"
#include "NavigationSystem.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "LGUEDK/Public/AI/NPC/NPCBase/NPCBaseController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Random Location";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ANPCBaseController* const Controller = Cast<ANPCBaseController>(OwnerComp.GetAIOwner());
    if (!Controller){return EBTNodeResult::Failed;}
    APawn* const Enemy = Controller->GetPawn();
    if (!Enemy){return EBTNodeResult::Failed;}

    bWantExplore = Cast<ANPCBaseStateEnemy>(Enemy)->GetDataAsset()->bWantExplore;
    FVector InitialSpawnPosition = OwnerComp.GetBlackboardComponent()->GetValueAsVector(InitialPositionKey.SelectedKeyName);
    FVector InitialPosition = Enemy->GetActorLocation();
    float SearchRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(SearchRadiusKey.SelectedKeyName);

    UNavigationSystemV1* const NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavigationSystem){return EBTNodeResult::Failed;}

    FNavLocation Result;
    FVector BestLocation = InitialSpawnPosition;

    if (!bWantExplore)
    {
        if (NavigationSystem->GetRandomReachablePointInRadius(InitialSpawnPosition, SearchRadius, Result))
        {
            BestLocation = Result.Location;
        }
    }
    else
    {
        if (NavigationSystem->GetRandomReachablePointInRadius(InitialPosition, SearchRadius, Result))
        {
            BestLocation = Result.Location;
        }
    }
    
    OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", BestLocation);
                
    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}


