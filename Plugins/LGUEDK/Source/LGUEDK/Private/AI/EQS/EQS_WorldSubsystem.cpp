// Copyright Villains, Inc. All Rights Reserved.


#include "AI/EQS/EQS_WorldSubsystem.h"

#include "MeshPaintVisualize.h"
#include "NavigationSystem.h"
#include "AI/EQS/EQSUtility.h"
#include "Algo/RandomShuffle.h"
#include "GameFramework/GameModeBase.h"


void UEQS_WorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
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
	UEQSUtility::InitWorldSubsystem(this);
	Init();
}

void UEQS_WorldSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UEQS_WorldSubsystem::Init()
{
	ClearRegisteredEntities();
	ClearEQSHelper();
}

void UEQS_WorldSubsystem::GenerateGridPoints(const FVector& TargetLocation,float InGridRadius, float InStepSize,float ZStep)
{
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSystem) return;

	const ANavigationData* NavData = NavSystem->GetDefaultNavDataInstance(FNavigationSystem::ECreateIfEmpty::DontCreate);
	if (!NavData) return;
	
	const FVector QueryExtent = FVector(InStepSize, InStepSize, ZStep);
	const float ZMaxStep = 10000;
	FNavLocation NavLocation;

	for (float X = -InGridRadius; X <= InGridRadius; X += InStepSize)
	{
		for (float Y = -InGridRadius; Y <= InGridRadius; Y += InStepSize)
		{
			for (float Z = -ZMaxStep; Z <= ZMaxStep; Z += ZStep)
			{
				const FVector SamplePoint = TargetLocation + FVector(X, Y, Z);

				if (NavSystem->ProjectPointToNavigation(SamplePoint, NavLocation, QueryExtent, NavData)
					&& IsPointFarFromNavEdge(NavSystem, NavLocation.Location, NavData))
				{
					const FPathFindingQuery Query(NavSystem, *NavData, TargetLocation, NavLocation.Location);

					if (NavSystem->TestPathSync(Query, EPathFindingMode::Regular))
					{
						bool bIsFarEnough = true;
						for (const FVector& Existing : ValidPoints)
						{
							if (FVector::DistSquared(Existing, NavLocation.Location) < 100.0f * 100.0f)
							{
								bIsFarEnough = false;
								break;
							}
						}

						if (bIsFarEnough)
						{
							ValidPoints.Add(NavLocation.Location);
						}
					}
				}
			}
		}
	}

#if !UE_BUILD_SHIPPING
	if (bShowDebug && !bWantUseAreasPoint)
	{
		for (const FVector& Point : ValidPoints)
		{
			DrawDebugSphere(
				GetWorld(), 
				Point,				
				50.0f,             
				12,                
				FColor::Green,     
				false,             
				100.0f           
			);
		}
	}
#endif

	if (bWantUseAreasPoint)
		DistributePointsToRegions();
}

void UEQS_WorldSubsystem::RegisterEntity(AActor* InEntity)
{
	if (!RegisteredEntities.Contains(InEntity))
	{
		RegisteredEntities.Add(InEntity);
		//UE_LOG(LogTemp, Log, TEXT("Registering entity: %s"), *InEntity->GetName());
	}
}

void UEQS_WorldSubsystem::UnregisterEntity(AActor* InEntity)
{
	if (RegisteredEntities.Contains(InEntity))
	{
		//UE_LOG(LogTemp, Log, TEXT("Unregistering entity: %s"), *InEntity->GetName());
		RegisteredEntities.Remove(InEntity);
	}
}

TArray<FVector> UEQS_WorldSubsystem::GetPoints(const FVector& TargetLocation, float InMinDistance, float InMaxDistance)
{
	if (ValidPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid points available"));
		return TArray<FVector>();
	}
	
	TArray<FVector> FilteredPoints;
	
	for (FVector& Point : ValidPoints)
	{
		if (IsInRange(TargetLocation, Point, InMinDistance, InMaxDistance))
		{
			FilteredPoints.Add(Point);
		}
	}

	return FilteredPoints;
}

FVector UEQS_WorldSubsystem::GetRandomExplorationPoint(const AActor* SelfEntity,const AActor* TargetIgnored, const FVector& InitialSpawnPosition,const FVector& CurrentPosition, float SearchRadius, bool bWantExplore)
{
	FVector SearchOrigin = bWantExplore ? CurrentPosition : InitialSpawnPosition;
	
	TArray<FVector> FilteredPoints = GetPoints(SearchOrigin, 0.0f, SearchRadius);
	
	if (FilteredPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid points available for exploration"));
		return SearchOrigin;
	}

	Algo::RandomShuffle(FilteredPoints);
	for (FVector FilteredPoint : FilteredPoints)
	{
		if (IsPointFree(SelfEntity,TargetIgnored,FilteredPoint))
		{
			return FilteredPoint;
		}
	}

	return SearchOrigin;
}

FVector UEQS_WorldSubsystem::GetBoidExplorationPoint(const AActor* SelfEntity, const FVector& InitialSpawnPosition,const FVector& CurrentPosition, float SearchRadius, bool bWantExplore)
{
	FVector SearchOrigin = bWantExplore ? CurrentPosition : InitialSpawnPosition;
	
	TArray<FVector> FilteredPoints = GetPoints(SearchOrigin, 0.0f, SearchRadius);
	
	if (FilteredPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid exploration points found"));
		return SearchOrigin;
	}

	// === BOID LOGIC ===
	const FVector SelfLocation = SelfEntity->GetActorLocation();
	FVector CohesionVec = FVector::ZeroVector;
	FVector AlignmentVec = FVector::ZeroVector;
	FVector SeparationVec = FVector::ZeroVector;
	int32 NeighborCount = 0;
	
	//Analizza tutte le altre entità registrate per applicare la logica boid
	for (AActor* Other : RegisteredEntities)
	{
		if (!IsValid(Other) || Other == SelfEntity) continue;

		const FVector OtherLoc = Other->GetActorLocation();
		const float Dist = FVector::Dist(SelfLocation, OtherLoc);

		// Entità entro la distanza utile per il comportamento boid
		if (Dist < BoidViewRadius)
		{
			CohesionVec += OtherLoc;

			// Allineamento: direzione media degli altri agenti
			if (Other->GetVelocity().SizeSquared() > 0)
			{
				AlignmentVec += Other->GetVelocity().GetSafeNormal();
			}

			// Separazione: allontanati se troppo vicino
			if (Dist < BoidSeparationRadius)
			{
				SeparationVec += (SelfLocation - OtherLoc).GetSafeNormal() / Dist;
			}

			NeighborCount++;
		}
	}

	// Calcola i vettori medi solo se ci sono vicini
	if (NeighborCount > 0)
	{
		CohesionVec = (CohesionVec / NeighborCount - SelfLocation).GetSafeNormal();
		AlignmentVec = (AlignmentVec / NeighborCount).GetSafeNormal();
		SeparationVec = SeparationVec.GetSafeNormal();
	}
	else
	{
		// Nessun vicino → dirigiti verso l’origine della ricerca
		CohesionVec = (SearchOrigin - SelfLocation).GetSafeNormal();
	}

	// Combina i tre vettori pesati
	FVector FinalSteer = CohesionVec * BoidWeightCohesion +
						 AlignmentVec * BoidWeightAlignment +
						 SeparationVec * BoidWeightSeparation;

	// Ordina i punti in base all'allineamento con la direzione preferita (FinalSteer)
	FilteredPoints.Sort([&](const FVector& A, const FVector& B)
	{
		const FVector DirA = (A - SelfLocation).GetSafeNormal();
		const FVector DirB = (B - SelfLocation).GetSafeNormal();
		return FVector::DotProduct(FinalSteer, DirA) > FVector::DotProduct(FinalSteer, DirB);
	});


	FVector BestLocation = FilteredPoints[0];

#if !UE_BUILD_SHIPPING
	if (bShowDebug)
	{
		//Origine della ricerca
		DrawDebugSphere(GetWorld(), SearchOrigin, 50.0f, 12, FColor::Blue, false, 5.0f);
		//Posizione dell'entità
		DrawDebugSphere(GetWorld(), SelfLocation, 30.0f, 12, FColor::Green, false, 5.0f);
		//Direzione risultante del vettore finale
		DrawDebugLine(GetWorld(), SelfLocation, SelfLocation + FinalSteer * 300.0f, FColor::Red, false, 5.0f, 0, 2.0f);
		DrawDebugString(GetWorld(), SelfLocation + FinalSteer * 300.0f, TEXT("FinalSteer"), nullptr, FColor::Red, 5.0f);
		//Tutti i punti considerati
		for (int32 i = 0; i < FilteredPoints.Num(); i++)
		{
			DrawDebugSphere(GetWorld(), FilteredPoints[i], 20.0f, 8, FColor::Yellow, false, 5.0f);
		}
		//Punto scelto finale (dopo jitter)
		DrawDebugSphere(GetWorld(), BestLocation, 40.0f, 12, FColor::Cyan, false, 5.0f);
		DrawDebugString(GetWorld(), BestLocation + FVector(0, 0, 50), TEXT("Selected Point"), nullptr, FColor::Cyan, 5.0f);
	}
#endif
	
	return BestLocation;
}

FVector UEQS_WorldSubsystem::GetAttackPoint(const AActor* SelfEntity,const AActor* TargetIgnored,const FVector& TargetLocation, float InMinDistance, float InMaxDistance,const float Margin)
{
	FVector ControlledPawnPosition = SelfEntity->GetActorLocation();
	FVector PlayerPosition = TargetLocation;
	FVector DesiredDirection = (ControlledPawnPosition - PlayerPosition).GetSafeNormal();
	
	auto FindValidPoint = [&](const TArray<FVector>& Points) -> FVector
	{
		for (const FVector& Point : Points)
		{
			const FVector DirectionToPoint = (Point - PlayerPosition).GetSafeNormal();
			const float DotProduct = FVector::DotProduct(DesiredDirection, DirectionToPoint);

			if (DotProduct >= Margin && !IsHittingSomething(SelfEntity,TargetIgnored,Point, TargetLocation) && IsPointFree(SelfEntity,TargetIgnored,Point))
			{
				return Point;
			}
		}
		return ControlledPawnPosition;
	};

	
	TArray<FVector> Points = GetPoints(PlayerPosition, InMinDistance, InMaxDistance);
	Algo::RandomShuffle(Points);
	FVector SelectedPoint = FindValidPoint(Points);

	if (!SelectedPoint.IsZero())
	{
		return SelectedPoint;
	}
	
	Points = UEQSUtility::GetPoints(PlayerPosition, InMinDistance / 2, InMaxDistance / 2);
	Algo::RandomShuffle(Points);
	SelectedPoint = FindValidPoint(Points);

	return SelectedPoint;
}

void UEQS_WorldSubsystem::SetRegion(AEQS_Helper* InEQSHelper, const FGameplayTag& InTag)
{
	if (EQSHelper.Contains(InEQSHelper))return;
	EQSHelper.Add(InEQSHelper);
	RegionPoints.Add(InTag);
	RegionPoints[InTag].Points = TArray<FVector>();
}

TArray<FVector> UEQS_WorldSubsystem::GetAreaPoints(const FVector& TargetLocation, float InMinDistance, float InMaxDistance,const FGameplayTag& InTag)
{
	if (!RegionPoints.Contains(InTag))
	{
		UE_LOG(LogTemp, Warning, TEXT("Region not found"));
		return TArray<FVector>();
	}
	
	if (RegionPoints[InTag].Points.Num() == 0) return TArray<FVector>();
	
	TArray<FVector> FilteredPoints;
	
	for (FVector& Point : RegionPoints[InTag].Points)
	{
		if (IsInRange(TargetLocation, Point, InMinDistance, InMaxDistance))
		{
			FilteredPoints.Add(Point);
		}
	}
	return FilteredPoints;
}

FVector UEQS_WorldSubsystem::GetRandomExplorationAreaPoint(const AActor* SelfEntity,const AActor* TargetIgnored, const FVector& InitialSpawnPosition,const FVector& CurrentPosition, float SearchRadius, bool bWantExplore, const FGameplayTag& InTag)
{
	FVector SearchOrigin = bWantExplore ? CurrentPosition : InitialSpawnPosition;
	
	TArray<FVector> FilteredPoints = GetAreaPoints(SearchOrigin, 0.0f, SearchRadius,InTag);
	
	if (FilteredPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid points available for exploration"));
		return SearchOrigin;
	}

	Algo::RandomShuffle(FilteredPoints);
	for (FVector FilteredPoint : FilteredPoints)
	{
		if (IsPointFree(SelfEntity,TargetIgnored,FilteredPoint))
		{
			return FilteredPoint;
		}
	}

	return SearchOrigin;
}

FVector UEQS_WorldSubsystem::GetBoidExplorationAreaPoint(const AActor* SelfEntity, const FVector& InitialSpawnPosition,const FVector& CurrentPosition, float SearchRadius, bool bWantExplore, const FGameplayTag& InTag)
{
	FVector SearchOrigin = bWantExplore ? CurrentPosition : InitialSpawnPosition;
	
	TArray<FVector> FilteredPoints = GetAreaPoints(SearchOrigin, 0.0f, SearchRadius,InTag);
	
	if (FilteredPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid exploration points found"));
		return SearchOrigin;
	}

	// === BOID LOGIC ===
	const FVector SelfLocation = SelfEntity->GetActorLocation();
	FVector CohesionVec = FVector::ZeroVector;
	FVector AlignmentVec = FVector::ZeroVector;
	FVector SeparationVec = FVector::ZeroVector;
	int32 NeighborCount = 0;

	//Analizza tutte le altre entità registrate per applicare la logica boid
	for (AActor* Other : RegisteredEntities)
	{
		if (!IsValid(Other) || Other == SelfEntity) continue;

		const FVector OtherLoc = Other->GetActorLocation();
		const float Dist = FVector::Dist(SelfLocation, OtherLoc);

		// Entità entro la distanza utile per il comportamento boid
		if (Dist < BoidViewRadius)
		{
			CohesionVec += OtherLoc;

			// Allineamento: direzione media degli altri agenti
			if (Other->GetVelocity().SizeSquared() > 0)
			{
				AlignmentVec += Other->GetVelocity().GetSafeNormal();
			}

			// Separazione: allontanati se troppo vicino
			if (Dist < BoidSeparationRadius)
			{
				SeparationVec += (SelfLocation - OtherLoc).GetSafeNormal() / Dist;
			}

			NeighborCount++;
		}
	}

	// Calcola i vettori medi solo se ci sono vicini
	if (NeighborCount > 0)
	{
		CohesionVec = (CohesionVec / NeighborCount - SelfLocation).GetSafeNormal();
		AlignmentVec = (AlignmentVec / NeighborCount).GetSafeNormal();
		SeparationVec = SeparationVec.GetSafeNormal();
	}
	else
	{
		// Nessun vicino → dirigiti verso l’origine della ricerca
		CohesionVec = (SearchOrigin - SelfLocation).GetSafeNormal();
	}
	
	// Combina i tre vettori pesati
	FVector FinalSteer = CohesionVec * BoidWeightCohesion +
						 AlignmentVec * BoidWeightAlignment +
						 SeparationVec * BoidWeightSeparation;

	// Ordina i punti in base all'allineamento con la direzione preferita (FinalSteer)
	FilteredPoints.Sort([&](const FVector& A, const FVector& B)
	{
		const FVector DirA = (A - SelfLocation).GetSafeNormal();
		const FVector DirB = (B - SelfLocation).GetSafeNormal();
		return FVector::DotProduct(FinalSteer, DirA) > FVector::DotProduct(FinalSteer, DirB);
	});


	FVector BestLocation = FilteredPoints[0];

#if !UE_BUILD_SHIPPING
	if (bShowDebug)
	{
		//Origine della ricerca
		DrawDebugSphere(GetWorld(), SearchOrigin, 50.0f, 12, FColor::Blue, false, 5.0f);
		//Posizione dell'entità
		DrawDebugSphere(GetWorld(), SelfLocation, 30.0f, 12, FColor::Green, false, 5.0f);
		//Direzione risultante del vettore finale
		DrawDebugLine(GetWorld(), SelfLocation, SelfLocation + FinalSteer * 300.0f, FColor::Red, false, 5.0f, 0, 2.0f);
		DrawDebugString(GetWorld(), SelfLocation + FinalSteer * 300.0f, TEXT("FinalSteer"), nullptr, FColor::Red, 5.0f);
		//Tutti i punti considerati
		for (int32 i = 0; i < FilteredPoints.Num(); i++)
		{
			DrawDebugSphere(GetWorld(), FilteredPoints[i], 20.0f, 8, FColor::Yellow, false, 5.0f);
		}
		//Punto scelto finale (dopo jitter)
		DrawDebugSphere(GetWorld(), BestLocation, 40.0f, 12, FColor::Cyan, false, 5.0f);
		DrawDebugString(GetWorld(), BestLocation + FVector(0, 0, 50), TEXT("Selected Point"), nullptr, FColor::Cyan, 5.0f);
	}
#endif
	
	return BestLocation;

}

FVector UEQS_WorldSubsystem::GetAttackAreaPoint(const AActor* SelfEntity,const AActor* TargetIgnored,const FVector& TargetLocation, float InMinDistance,float InMaxDistance, const float Margin, const FGameplayTag& InTag)
{
	const FVector ControlledPawnPosition = SelfEntity->GetActorLocation();
	const FVector DesiredDirection = (ControlledPawnPosition - TargetLocation).GetSafeNormal();

	auto FindValidPoint = [&](const TArray<FVector>& Points) -> FVector
	{
		for (const FVector& Point : Points)
		{
			const FVector DirectionToPoint = (Point - TargetLocation).GetSafeNormal();
			const float DotProduct = FVector::DotProduct(DesiredDirection, DirectionToPoint);

			if (DotProduct >= Margin &&
				!IsHittingSomething(SelfEntity, TargetIgnored, Point, TargetLocation) &&
				IsPointFree(SelfEntity, TargetIgnored, Point))
			{
				return Point;
			}
		}
		return FVector::ZeroVector;
	};

	TArray<FVector> Points = GetAreaPoints(TargetLocation, InMinDistance, InMaxDistance, InTag);
	Algo::RandomShuffle(Points);

	FVector SelectedPoint = FindValidPoint(Points);
	if (!SelectedPoint.IsZero())
	{
		return SelectedPoint;
	}
	
	Points = UEQSUtility::GetAreaPoints(TargetLocation, InMinDistance / 2, InMaxDistance / 2, InTag);
	Algo::RandomShuffle(Points);
	SelectedPoint = FindValidPoint(Points);
	
	return !SelectedPoint.IsZero() ? SelectedPoint : TargetLocation;
}

FVector UEQS_WorldSubsystem::GetChasingAreaPoint(const AActor* SelfEntity,const AActor* TargetIgnored,const FVector& TargetLocation, float InMinDistance, float InMaxDistance)
{
	FGameplayTag RegionTag = GetTagFromPoint(TargetLocation);
	
	TArray<FVector> PossiblePoints = GetAreaPoints(TargetLocation, InMinDistance, InMaxDistance, RegionTag);
	
	auto FindValidPoint = [&](const TArray<FVector>& Points) -> FVector
	{
		for (const FVector& Point : Points)
		{
			if (!IsHittingSomething(SelfEntity,TargetIgnored,Point, TargetLocation) && IsPointFree(SelfEntity,TargetIgnored,Point))
			{
				return Point;
			}
		}
		return TargetLocation;
	};

	Algo::RandomShuffle(PossiblePoints);
	FVector SelectedPoint = FindValidPoint(PossiblePoints);

	if (!SelectedPoint.IsZero())
	{
		return SelectedPoint;
	}
	
	PossiblePoints = UEQSUtility::GetAreaPoints(TargetLocation, InMinDistance / 2, InMaxDistance / 2,RegionTag);
	Algo::RandomShuffle(PossiblePoints);
	SelectedPoint = FindValidPoint(PossiblePoints);

	return SelectedPoint;
}

FVector UEQS_WorldSubsystem::GetChasingPoint(const AActor* SelfEntity,const AActor* TargetIgnored,const FVector& TargetLocation, float InMinDistance, float InMaxDistance)
{
	TArray<FVector> PossiblePoints =  GetPoints(TargetLocation, InMinDistance, InMaxDistance);
	
	if (PossiblePoints.Num() == 0)
	{
		return TargetLocation;
	}
	
	Algo::RandomShuffle(PossiblePoints);
	for (FVector FilteredPoint : PossiblePoints)
	{
		if (IsPointFree(SelfEntity,TargetIgnored,FilteredPoint) && !IsHittingSomething(SelfEntity,TargetIgnored, FilteredPoint, TargetLocation))
		{
			return FilteredPoint;
		}
	}
	return PossiblePoints[0];
}

void UEQS_WorldSubsystem::ClearRegisteredEntities()
{
	RegisteredEntities.Empty();
}

void UEQS_WorldSubsystem::ClearEQSHelper()
{
	RegionPoints.Empty();
	EQSHelper.Empty();
}

void UEQS_WorldSubsystem::DistributePointsToRegions()
{
	TMap<FGameplayTag, FColor> RegionColors;
	TMap<FVector, TArray<FGameplayTag>> PointRegionMap;

	if (EQSHelper.Num() == 0)
	{
		return;
	}
	
	for (const FVector& Point : ValidPoints)
	{
		for (AEQS_Helper* Helper : EQSHelper)
		{
			if (Helper && Helper->IsInsideRegion(Point))
			{
				FGameplayTag RegionTag = Helper->GetRegionTag();
				RegionPoints.FindOrAdd(RegionTag).Points.Add(Point);

				PointRegionMap.FindOrAdd(Point).AddUnique(RegionTag);
			}
		}
	}

#if !UE_BUILD_SHIPPING
	if (bShowDebug)
	{
		UE_LOG(LogTemp, Log, TEXT("=== REGION POINTS SUMMARY ==="));
		for (const auto& Elem : RegionPoints)
		{
			const FGameplayTag& RegionTag = Elem.Key;
			const int32 NumPoints = Elem.Value.Points.Num();
			UE_LOG(LogTemp, Log, TEXT("Region: %s -> Points: %d"), *RegionTag.ToString(), NumPoints);
		}

		for (const auto& PointPair : PointRegionMap)
		{
			const FVector& Point = PointPair.Key;
			const TArray<FGameplayTag>& Tags = PointPair.Value;

			FColor DebugColor;
			if (Tags.Num() > 1)
			{
				DebugColor = FColor::Magenta;
			}
			else
			{
				const FGameplayTag& SingleTag = Tags[0];
				if (!RegionColors.Contains(SingleTag))
				{
					RegionColors.Add(SingleTag, FColor::MakeRandomColor());
				}
				DebugColor = RegionColors[SingleTag];
			}

			DrawDebugSphere(GetWorld(), Point, 40.0f, 12, DebugColor, false, 3000.0f);
		}
	}
#endif
}

bool UEQS_WorldSubsystem::IsInRange(const FVector& TargetPosition,const FVector& CurrentPosition, const float InMinDistance, const float InMaxDistance)
{
	float Distance = FVector::Distance(TargetPosition, CurrentPosition);
	return Distance >= InMinDistance && Distance <= InMaxDistance;
}

bool UEQS_WorldSubsystem::IsPointFarFromNavEdge(const UNavigationSystemV1* NavSystem, const FVector& Point,const ANavigationData* NavData)
{
	if (!NavSystem || !NavData)
	{
		return false;
	}
	
	FNavLocation Projected;
	if (!NavSystem->ProjectPointToNavigation(Point, Projected, FVector(DistanceFromNavmeshEdge, DistanceFromNavmeshEdge, 100.0f), NavData))
	{
		return false;
	}
	
	const TArray<FVector> Directions = {
		FVector(DistanceFromNavmeshEdge, 0, 0),
		FVector(-DistanceFromNavmeshEdge, 0, 0),
		FVector(0, DistanceFromNavmeshEdge, 0),
		FVector(0, -DistanceFromNavmeshEdge, 0),
		FVector(DistanceFromNavmeshEdge, DistanceFromNavmeshEdge, 0),
		FVector(-DistanceFromNavmeshEdge, DistanceFromNavmeshEdge, 0),
		FVector(DistanceFromNavmeshEdge, -DistanceFromNavmeshEdge, 0),
		FVector(-DistanceFromNavmeshEdge, -DistanceFromNavmeshEdge, 0)
	};

	int32 ValidCount = 0;

	for (const FVector& Offset : Directions)
	{
		FNavLocation Nearby;
		if (NavSystem->ProjectPointToNavigation(Point + Offset, Nearby, FVector(DistanceFromNavmeshEdge * 0.5f), NavData))
		{
			ValidCount++;
		}
	}
	
	return ValidCount >= 8;
}

bool UEQS_WorldSubsystem::IsHittingSomething(const AActor* SelfIgnored,const AActor* TargetIgnored,const FVector& Start, const FVector& End) const
{
	FVector StartPosition = Start + FVector(0, 0, 100);
	FVector EndPosition = End + FVector(0, 0, 100);

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(SelfIgnored);
	TraceParams.AddIgnoredActor(TargetIgnored);

	FCollisionShape SphereShape = FCollisionShape::MakeSphere(55.0f);

	FHitResult HitResult;
	bool bHit = GetWorld()->SweepSingleByChannel
		(
		HitResult,
		StartPosition,
		EndPosition,
		FQuat::Identity,
		CollisionChannelIsHittingSomething,
		SphereShape,
		TraceParams
		);

#if !UE_BUILD_SHIPPING
	if (bShowDebug)
	{
		float SphereRadius = 55.0f;

		DrawDebugLine(
			GetWorld(),
			StartPosition,
			EndPosition,
			bHit ? FColor::Black : FColor::Green,
			false,
			2.0f,
			0,
			SphereRadius 
		);
	}
#endif

	return bHit; 
}

bool UEQS_WorldSubsystem::IsPointFree(const AActor* SelfIgnored,const AActor* TargetIgnored,const FVector& Point) const
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(SelfIgnored);
	QueryParams.AddIgnoredActor(TargetIgnored);
	
	FCollisionObjectQueryParams TraceParams;
	TraceParams.AddObjectTypesToQuery(ObjectTypeChannelPointFree);
	
	FHitResult HitResult;
	const bool bHit = GetWorld()->SweepSingleByObjectType(
		HitResult, 
		Point, 
		Point + FVector(0, 0, 10), 
		FQuat::Identity, 
		TraceParams, 
		FCollisionShape::MakeSphere(DistanceBetweenEnemyRadius), 
		QueryParams
	);
	
	const bool bPointIsFree = !bHit;

#if !UE_BUILD_SHIPPING
	if (bShowDebug)
	{
		const FColor SphereColor = bPointIsFree ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), Point, DistanceBetweenEnemyRadius, 12, SphereColor, false, 2.0f);
	}
#endif	
	return bPointIsFree;
}

FGameplayTag UEQS_WorldSubsystem::GetTagFromPoint(const FVector& Point)
{
	AEQS_Helper* ClosestHelper = nullptr;

	for (AEQS_Helper* Helper : EQSHelper)
	{
		if (!IsValid(Helper))
		{
			continue;
		}

		if (Helper->IsInsideRegion(Point))
		{
			ClosestHelper = Helper;
			break;
		}
	}
	FGameplayTag RegionTag = ClosestHelper ? ClosestHelper->GetRegionTag() : FGameplayTag();

	return RegionTag;
}


