// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_FindNavMeshPoint.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NavMesh/RecastNavMesh.h"
#include "Utility/LGDebug.h"


UBTTask_FindNavMeshPoint::UBTTask_FindNavMeshPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find NavMesh Point";
}

EBTNodeResult::Type UBTTask_FindNavMeshPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;
	
    AActor* ControlledPawn = AIController->GetPawn();
	if (ControlledPawn == nullptr) return EBTNodeResult::Failed;

    AActor* AttackTarget =Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName)); 
    FVector PlayerLocation = AttackTarget->GetActorLocation();
    float MinDistance = BlackboardComp->GetValueAsFloat(MinDistanceKey.SelectedKeyName);
    float MaxDistance = BlackboardComp->GetValueAsFloat(MaxDistanceKey.SelectedKeyName);

    FVector TargetLocation = FindFarthestPointWithinRange(PlayerLocation,ControlledPawn,MinDistance, MaxDistance);

    BlackboardComp->SetValueAsVector(TargetLocationKey.SelectedKeyName,TargetLocation);
    
    FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}

FVector UBTTask_FindNavMeshPoint::FindFarthestPointWithinRange(const FVector& PlayerLocation,const AActor* Enemy,const float& MinDistance,const float& MaxDistance)
{
    FVector BestPoint = FVector::Zero();
    TArray<FNavLocation> NavPoints;
    TArray<FNavLocation> CurrentPossiblePoints;
    FNavLocation NavLocation;

    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (!NavSystem) return BestPoint;

    NavPoints.Empty();
    
    GetAllReachableNavPoints(NavSystem, PlayerLocation, MaxDistance, NavPoints);
    
    if (NavPoints.Num() == 0)
    {
        if (NavSystem->GetRandomReachablePointInRadius(PlayerLocation, MaxDistance, NavLocation))
        {
            NavPoints.Add(NavLocation);
            // LGDebug::Log("NavLocation: " + NavLocation.Location.ToString(), true);
            // LGDebug::Log("NavPoints. : " + FString::SanitizeFloat(NavPoints.Num()), true);
        }
    }
    
    for (const FNavLocation& NavPoint : NavPoints)
    {
        float Distance = FVector::Distance(PlayerLocation, NavPoint.Location);
    
        if (IsInRange(Distance, MinDistance, MaxDistance + 400) && !IsHittingSomething(NavPoint.Location, PlayerLocation, Enemy))
        {
            // LGDebug::Log("DISTANZA CORRETTA : " + NavPoint.Location.ToString(), true);
            // LGDebug::Log("NON HO HITTATO NULLA : " + NavPoint.Location.ToString(), true);
            
            CurrentPossiblePoints.Add(NavPoint);
        }
    }
    
    if (CurrentPossiblePoints.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, CurrentPossiblePoints.Num() - 1);
        BestPoint= CurrentPossiblePoints[RandomIndex];
    }
    else
    {
        BestPoint = PlayerLocation;
    }
    
    return BestPoint;
    
     
    
    
    // if (FVector::Dist(Enemy->GetActorLocation(), BestPoint) < 100.0f)  
    // {
    //     FVector AlternativePoint = FVector::ZeroVector;
    //     float ClosestDistanceToBestPoint = FLT_MAX;
    //     
    //     for (const FNavLocation& NavPoint : NavPoints)
    //     {
    //         float DistanceToBestPoint = FVector::Distance(BestPoint, NavPoint.Location);
    //         float DistanceToPlayer = FVector::Distance(PlayerLocation, NavPoint.Location);
    //         
    //         if (IsInRange(DistanceToPlayer, MinDistance, MaxDistance) &&
    //             !IsHittingSomething(NavPoint.Location, PlayerLocation, Enemy) &&
    //             DistanceToBestPoint > 50 && 
    //             DistanceToBestPoint < ClosestDistanceToBestPoint)
    //         {
    //             ClosestDistanceToBestPoint = DistanceToBestPoint;
    //             AlternativePoint = NavPoint.Location;
    //         }
    //     }
    //
    //     // Se troviamo un altro punto valido, usalo
    //     if (AlternativePoint != FVector::ZeroVector)
    //     {
    //         BestPoint = AlternativePoint;
    //         LGDebug::Log("ALTERNATIVE POINT FOUND : " + BestPoint.ToString(), true);
    //     }
    //     else
    //     {
    //         // Se non ci sono punti alternativi, scegli un punto casuale
    //         FNavLocation RandomNavPoint;
    //         if (NavSystem->GetRandomReachablePointInRadius(PlayerLocation, MaxDistance, RandomNavPoint))
    //         {
    //             BestPoint = RandomNavPoint.Location;
    //             LGDebug::Log("PUNTO CASUALE UTILIZZATO : " + BestPoint.ToString(), true);
    //         }
    //     }
    // }

    //return BestPoint;
    
    // if (NavSystem->GetRandomReachablePointInRadius(PlayerLocation, MaxDistance, NavLocation))
    // {
    //     NavPoints.Add(NavLocation);
    //     
    //     LGDebug::Log("NavPoints. : " + FString::SanitizeFloat(NavPoints.Num()), true);
    // }
    //
    // if (NavSystem->GetRandomReachablePointInRadius(PlayerLocation ,MaxDistance, NavLocation))
    // {
    //     PlayerNavMeshLocation = NavLocation.Location;
    //     LGDebug::Log("player location Z (World) : " + FString::SanitizeFloat(PlayerNavMeshLocation.Z), true);
    // }
    // else LGDebug::Log("player location Z (World) NON TROVATO: " + FString::SanitizeFloat(PlayerNavMeshLocation.Z), true);
    //
    // if (bSamePlane)
    // {
    //     BestPoint = GetCorrectNavPoint(NavPoints,BestDistance,PlayerLocation,PlayerNavMeshLocation,MinDistance,MaxDistance,Enemy); 
    // }
    // else
    // {
    //     for (const FNavLocation& NavPoint : NavPoints)
    //     {
    //         float Distance = FVector::Distance(PlayerLocation, NavPoint.Location);
    //         if (IsInRange(Distance,MinDistance,MaxDistance) && !IsHittingSomething(NavPoint.Location, PlayerLocation, Enemy))
    //         {
    //             if (Distance > BestDistance)
    //             {
    //                 BestDistance = Distance;
    //                 BestPoint = NavPoint.Location;
    //            }
    //         }
    //     }
    // }
    //
    // if (BestDistance == -1.0f)
    // {
    //     BestPoint = GetCorrectNavPoint(NavPoints,BestDistance,PlayerLocation,PlayerNavMeshLocation,MinDistance,MaxDistance,Enemy); 
    // }
    
}

bool UBTTask_FindNavMeshPoint::IsHittingSomething(const FVector& Start, const FVector& End,const AActor* Enemy)
{
    FHitResult HitResult;

    FVector StartLocation = Start + FVector(0,0,50);
    FVector EndLocation = End + FVector(0,0,50);
    
    FCollisionQueryParams TraceParams;
    TraceParams.bTraceComplex = true; 
    TraceParams.bReturnPhysicalMaterial = true; 
    TraceParams.AddIgnoredActor(Enemy);  
            
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult, 
        StartLocation,
        EndLocation,
        ECC_Visibility,
        TraceParams
    );
    //DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 5.0f, 0, 1.0f);  // Linea rossa
    return bHit;
}

bool UBTTask_FindNavMeshPoint::IsInRange(float& Distance,const float& MinDistance,const float& MaxDistance)
{
    return Distance > MinDistance && Distance < MaxDistance;
}

FVector UBTTask_FindNavMeshPoint::GetCorrectNavPoint(TArray<FNavLocation>& NavPoints,float& BestDistance, const FVector& PlayerLocation,FVector PlayerNavMeshLocation,const float& MinDistance,const float& MaxDistance,const AActor* Enemy)
{
    
    FVector BestPoint = FVector::ZeroVector;
    //
    // for (const FNavLocation& NavPoint : NavPoints)
    // {
    //     float Distance = FVector::Distance(PlayerLocation, NavPoint.Location);
    //         
    //     if (IsInRange(Distance,MinDistance,MaxDistance) && !IsHittingSomething(NavPoint.Location, PlayerLocation, Enemy) )
    //     {
    //         LGDebug::Log("Raycast non colpisce nulla: punto valido", true);
    //         if (Distance > BestDistance)
    //         {
    //             LGDebug::Log("player location : " + FString::SanitizeFloat(PlayerLocation.Z), true);
    //             LGDebug::Log("Punto considerato valido Z: " + FString::SanitizeFloat(NavPoint.Location.Z), true);
    //             BestDistance = Distance;
    //             BestPoint = NavPoint.Location;
    //         }
    //     }
    // }
    // return BestPoint;
    
    return BestPoint;
}

void UBTTask_FindNavMeshPoint::GetAllReachableNavPoints(UNavigationSystemV1* NavSystem, const FVector& Center,const float& Radius, TArray<FNavLocation>& OutNavPoints)
{
    const float GridRadius = Radius + 400; 
    const float StepSize = 500;           
    const int32 GridSize = FMath::CeilToInt(GridRadius / StepSize); 
    
    for (int32 X = -GridSize; X <= GridSize; ++X)
    {
        for (int32 Y = -GridSize; Y <= GridSize; ++Y)
        {
            FVector CandidatePoint = Center + FVector(X * StepSize, Y * StepSize, 0.0f);

            FNavLocation NavLocation;
            if (NavSystem->ProjectPointToNavigation(CandidatePoint, NavLocation, FVector(StepSize, StepSize, 200.0f)))
            {
                
                OutNavPoints.Add(NavLocation);
                //LGDebug::Log("Found Reachable Point: " + NavLocation.Location.ToString(), true);
            }
        }
    }

    
    // for (const FNavLocation& NavPoint : OutNavPoints)
    // {
    //     DrawDebugSphere(
    //         GetWorld(), 
    //         NavPoint.Location, // Posizione della sfera
    //         50.0f,             // Raggio della sfera
    //         12,                // Numero di segmenti
    //         FColor::Green,     // Colore della sfera
    //         false,             // Persistente nel tempo
    //         5.0f               // Durata della sfera
    //     );
    // }
}




