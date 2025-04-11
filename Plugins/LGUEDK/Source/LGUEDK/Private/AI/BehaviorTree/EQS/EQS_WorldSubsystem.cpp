// Copyright Villains, Inc. All Rights Reserved.


#include "AI/EQS/EQS_WorldSubsystem.h"

#include "MeshPaintVisualize.h"
#include "NavigationSystem.h"
#include "AI/EQS/EQSUtility.h"
#include "GameFramework/GameModeBase.h"


void UEQS_WorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UEQSUtility::InitWorldSubsystem(this);
	UE_LOG(LogTemp, Log, TEXT("EQS World Subsystem Initialized"));
}

void UEQS_WorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	const UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Log, TEXT("EQS World Subsystem: Invalid world"));
		return;
	}
	
	const AGameModeBase* GameMode = World->GetAuthGameMode();
	
	if (!IsValid(GameMode))
	{
		UE_LOG(LogTemp, Log, TEXT("EQS World Subsystem: Invalid GameMode"));
		return;
	}
	
	if (!GameModes.Contains(GameMode->GetClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("EQS World Subsystem: GameMode not contained in EQS World Subsystem"));
		return;
	}

	Init();
}

void UEQS_WorldSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UEQS_WorldSubsystem::Init()
{
	CurrentUpdateTime = 0.0f;
	bIsUpdating = false;
	
}

void UEQS_WorldSubsystem::RegisterEntity(AActor* Requester,AActor* Target)
{
	if (Requester && !ActiveEntity.Contains(Requester))
	{
		UE_LOG(LogTemp, Log, TEXT("Enemy added to EQS subsystem"));
		
		if (Target)
		{
			if (!Points.Contains(Target))
			{
				FVector TargetLocation = Target->GetActorLocation();
				TArray<FVector> CurrentPoints = GenerateGridPoints(TargetLocation, GridSize, StepSize);
				Points.Add(Target);
				Points[Target].Points = CurrentPoints;
			}
			ActiveEntity.Add(Requester);
			bIsUpdating = true;
		}
	}
}

void UEQS_WorldSubsystem::UnregisterEntity(AActor* Requester)
{
	if (Requester && ActiveEntity.Contains(Requester))
	{
		ActiveEntity.Remove(Requester);
		if (ActiveEntity.Num() == 0)
		{
			Points.Empty();
			Points = TMap<AActor*, FEQSContainer>();
			bIsUpdating = false;
			UE_LOG(LogTemp, Warning, TEXT("No more enemies active. Stopping EQS updates."));
		}
	}
}

void UEQS_WorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsUpdating)
	{
		CurrentUpdateTime -= DeltaTime;
		if (CurrentUpdateTime <= 0.0f)
		{
			UpdatePoints();
			CurrentUpdateTime = UpdateInterval;
		}
	}
}

void UEQS_WorldSubsystem::UpdatePoints()
{
	UE_LOG(LogTemp, Warning, TEXT("Updating EQS Points"));

	for (auto& Element : Points)
	{
		FVector TargetLocation = Element.Key->GetActorLocation();
		Element.Value.Points = GenerateGridPoints(TargetLocation, GridSize, StepSize);
	}
}

TArray<FVector> UEQS_WorldSubsystem::GenerateGridPoints(FVector& TargetLocation,float InGridRadius, float InStepSize)
{
	TArray<FVector> InPoints;
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSystem) return InPoints;

	const ANavigationData* NavData = NavSystem->GetDefaultNavDataInstance(FNavigationSystem::ECreateIfEmpty::DontCreate);
	if (!NavData) return InPoints;
	
	FNavLocation NavLocation;

	for (float X = -InGridRadius; X <= InGridRadius; X += InStepSize)
	{
		for (float Y = -InGridRadius; Y <= InGridRadius; Y += InStepSize)
		{
			FVector CandidatePoint = TargetLocation + FVector(X, Y, 0);
			if (NavSystem->ProjectPointToNavigation(CandidatePoint, NavLocation,FVector(StepSize, StepSize, 1000.0f)))
			{
				const FPathFindingQuery Query(NavSystem, *NavData, TargetLocation, NavLocation.Location);
				
				if (NavSystem->TestPathSync(Query, EPathFindingMode::Regular))
				{
					InPoints.Add(NavLocation.Location);
				}
			}
		}
	}

#if !UE_BUILD_SHIPPING
	if (bShowDebug)
	{
		for (const FVector& Point : InPoints)
		{
			DrawDebugSphere(
				GetWorld(), 
				Point,				
				50.0f,             
				12,                
				FColor::Green,     
				false,             
				UpdateInterval           
			);
		}
	}
#endif
	
	return InPoints;
}

TArray<FVector> UEQS_WorldSubsystem::TryGetPoint(AActor* Requester,AActor* Target, float InMinDistance, float InMaxDistance)
{
	if (!Points.Contains(Target))
	{
		UE_LOG(LogTemp, Warning, TEXT("Target non presente nella mappa di punti!"));
		return Points[Target].Points;
	}
	
	TArray<FVector> ValidPoints = GetPoint(Requester,Points[Target].Points, Target, InMinDistance,InMaxDistance);

#if !UE_BUILD_SHIPPING
	if (bShowDebug)
	{
		for (const FVector& Point : ValidPoints)
		{
			DrawDebugSphere(
				GetWorld(), 
				Point,				
				50.0f,             
				12,                
				FColor::Red,     
				false,             
				UpdateInterval           
			);
		}
	}
#endif	
	
	return ValidPoints;
}

TStatId UEQS_WorldSubsystem::GetStatId() const
{
	return GetStatID();
}

TArray<FVector> UEQS_WorldSubsystem::GetPoint(AActor* Requester,TArray<FVector>& PossiblePoints, AActor* Target, float InMinDistance, float InMaxDistance)
{
	PossiblePoints.RemoveAll([](const FVector& Point) { return Point.IsZero(); });

	FVector TargetLocation = Target->GetActorLocation();
	TArray<FVector> ValidPoints;

	for (FVector& Point : PossiblePoints)
	{
		if (IsInRange(Point,TargetLocation,InMinDistance,InMaxDistance))
		{
			ValidPoints.Add(Point);
		}
	}
	
	return ValidPoints;
}

void UEQS_WorldSubsystem::RemovePoint(AActor* Target,const FVector& PointToRemove)
{
	if (Points.Contains(Target) && Points[Target].Points.Contains(PointToRemove))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Removing point from EQS subsystem"));
		Points[Target].Points.Remove(PointToRemove);
	}
}

bool UEQS_WorldSubsystem::IsInRange(FVector& TargetPosition,FVector& CurrentPosition, const float InMinDistance, const float InMaxDistance)
{
	float Distance = FVector::Distance(TargetPosition, CurrentPosition);
	return Distance >= InMinDistance && Distance <= InMaxDistance;
}


