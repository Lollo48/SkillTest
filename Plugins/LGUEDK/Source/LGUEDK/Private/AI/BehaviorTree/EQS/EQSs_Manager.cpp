// Copyright Villains, Inc. All Rights Reserved.


#include "AI/EQS/EQSs_Manager.h"

#include "NavigationSystem.h"
#include "Utility/LGDebug.h"


AEQSs_Manager::AEQSs_Manager()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentPoints = TMap<AActor*,TArray<FVector>>();
	bShowDebug = false;
	bNeedToUpdatePoints = false;
	CurrentUpdateTime = TimeToUpdatePoints;
	TimeToUpdatePoints = 0.0f;
}

void AEQSs_Manager::BeginPlay()
{
	Super::BeginPlay();
	CurrentUpdateTime = TimeToUpdatePoints;
}

void AEQSs_Manager::Tick(float DeltaTime)
{
	if (bNeedToUpdatePoints)
	{
		if (CurrentUpdateTime <= 0.0f)
		{
			UpdatePoints();
			CurrentUpdateTime = TimeToUpdatePoints;
		}
		CurrentUpdateTime -= DeltaTime;
	}
}

TArray<FVector> AEQSs_Manager::TryGetPossiblePoints(AActor* MyAttackTarget)
{
	if (!CurrentPoints.Contains(MyAttackTarget) || CurrentPoints[MyAttackTarget].Num() <= 0)
		SetBaseGridPoints(MyAttackTarget);
	
	bNeedToUpdatePoints = true;
	return CurrentPoints[MyAttackTarget];
}

void AEQSs_Manager::SetPossiblePoints(AActor* MyAttackTarget, TArray<FVector>& Points)
{
	if (!CurrentPoints.Contains(MyAttackTarget))
		CurrentPoints.Add(MyAttackTarget);
	
	CurrentPoints[MyAttackTarget].Empty();
	
	CurrentPoints[MyAttackTarget] = Points;
}

void AEQSs_Manager::RemoveUsablePoints(AActor* MyAttackTarget, FVector& Point)
{
	CurrentPoints[MyAttackTarget].Remove(Point);
}

void AEQSs_Manager::ClearMap(AActor* MyAttackTarget)
{
	CurrentPoints.Remove(MyAttackTarget);
	bNeedToUpdatePoints = false;
}

void AEQSs_Manager::SetBaseGridPoints(AActor* ActorGridOriginPosition)
{
	TArray<FVector> PossiblePoints;
	
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSystem) return;

	const ANavigationData* NavData = NavSystem->GetDefaultNavDataInstance(FNavigationSystem::ECreateIfEmpty::DontCreate);
	if (!NavData) return;
	
	const int32 GridSize = FMath::CeilToInt(GridRadius / StepSize); 
	
	FNavLocation NavLocation;

	FVector TargetLocation = ActorGridOriginPosition->GetActorLocation();
	
	const float MinDistanceSquared = FMath::Square(StepSize * 0.8f); 

	for (int32 X = -GridSize; X <= GridSize; ++X)
	{
		for (int32 Y = -GridSize; Y <= GridSize; ++Y)
		{
			FVector CandidatePoint = TargetLocation + FVector(X * StepSize, Y * StepSize, 0.0f);
        
			if (NavSystem->ProjectPointToNavigation(CandidatePoint, NavLocation, FVector(StepSize, StepSize, 10000.0f)))
			{
				const FPathFindingQuery Query(NavSystem, *NavData, TargetLocation, NavLocation.Location);
            
				if (NavSystem->TestPathSync(Query, EPathFindingMode::Regular))
				{
					bool bIsTooClose = false;
					for (const FVector& ExistingPoint : PossiblePoints)
					{
						if (FVector::DistSquared(ExistingPoint, NavLocation.Location) < MinDistanceSquared)
						{
							bIsTooClose = true;
							break;
						}
					}

					if (!bIsTooClose)
					{
						PossiblePoints.Add(NavLocation.Location);
					}
				}
			}
		}
	}

#if !UE_BUILD_SHIPPING
	if (bShowDebug)
	{
		for (const FVector& Point : PossiblePoints)
		{
			DrawDebugSphere(
				GetWorld(), 
				Point,				
				50.0f,             
				12,                
				FColor::Red,     
				false,             
				5           
			);
		}
	}
#endif
	
	SetPossiblePoints(ActorGridOriginPosition,PossiblePoints);
	
}

void AEQSs_Manager::UpdatePoints()
{
	if (CurrentPoints.Num() == 0)
	{
		bNeedToUpdatePoints = false;
		LGDebug::Log("UpdatePoints: Nessun attore presente in CurrentPoints", true);
		return;
	}

	for (const TPair<AActor*, TArray<FVector>>& Pair : CurrentPoints)
	{
		AActor* ActorGridOrigin = Pair.Key;
		if (!ActorGridOrigin) 
		{
			LGDebug::Log("UpdatePoints: Attore nullo nella mappa!", true);
			continue;
		}
		
		SetBaseGridPoints(ActorGridOrigin);
		
		if (bShowDebug)
			LGDebug::Log("UpdatePoints: Punto aggiornato con successo!", true);
	}
	
	if (bShowDebug)
		LGDebug::Log("UpdatePoints: Punti aggiornati con successo!", true);
}
