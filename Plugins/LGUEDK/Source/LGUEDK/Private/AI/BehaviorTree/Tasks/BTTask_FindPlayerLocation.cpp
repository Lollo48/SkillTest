// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_FindPlayerLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/LGDebug.h"

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find player location";
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get player character
	if (ACharacter* const playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(),0))
	{
		// get player location to use as origin
		FVector const playerLocation = playerCharacter->GetActorLocation();

		// LGDebug::Log(SearchRandom?"true":"false" ,true);
		
		if (SearchRandom)
		{
			//LGDebug::Log(SearchRandom?"true":"false" ,true);
			
			FNavLocation location;

			//get the navigation system and generate a random location near the player
			if (UNavigationSystemV1* const navigationSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				// try to get a random position near the player
				if (navigationSystem->GetRandomPointInNavigableRadius(playerLocation,SearchRadius,location))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation",location.Location);
					FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
				
			}
			
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation",playerLocation);
			FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
