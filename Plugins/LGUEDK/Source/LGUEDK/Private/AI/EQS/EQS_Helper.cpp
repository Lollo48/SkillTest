// Copyright Villains, Inc. All Rights Reserved.


#include "AI/EQS/EQS_Helper.h"

#include "AI/EQS/EQSUtility.h"
#include "AI/Manager/WaveSystem/Utility/WaveManagerUtility.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"


AEQS_Helper::AEQS_Helper()
{
	InfluenceBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InfluenceBox"));
	InfluenceBox->SetupAttachment(RootComponent);
	InfluenceBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InfluenceBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	InfluenceBox->SetGenerateOverlapEvents(true);
	InfluenceBox->SetBoxExtent(FVector(500.f));
}

bool AEQS_Helper::IsInsideRegion(const FVector& Point) const
{
	const FTransform& BoxTransform = InfluenceBox->GetComponentTransform();
	const FVector LocalPoint = BoxTransform.InverseTransformPosition(Point);
	return FMath::Abs(LocalPoint.X) <= InfluenceBox->GetUnscaledBoxExtent().X &&
		   FMath::Abs(LocalPoint.Y) <= InfluenceBox->GetUnscaledBoxExtent().Y &&
		   FMath::Abs(LocalPoint.Z) <= InfluenceBox->GetUnscaledBoxExtent().Z;
}

void AEQS_Helper::BeginPlay()
{
	Super::BeginPlay();
	UEQSUtility::SetRegion(this, RegionTag);
	
#if !UE_BUILD_SHIPPING
	if (bShowDebug && InfluenceBox)
	{
		const FVector BoxCenter = InfluenceBox->GetComponentLocation();
		const FVector BoxExtent = InfluenceBox->GetScaledBoxExtent();
		const FQuat BoxRotation = InfluenceBox->GetComponentQuat();
		
		DrawDebugBox(
			GetWorld(),
			BoxCenter,
			BoxExtent,
			BoxRotation,
			FColor::Yellow,
			false,
			5000.0f, 
			0,
			0 
		);
		
		DrawDebugString(
			GetWorld(),
			BoxCenter + FVector(0, 0, BoxExtent.Z + 100.0f),
			RegionTag.ToString(),
			nullptr,
			FColor::Yellow,
			5000.0f,
			false,
			2.0f
		);
	}
#endif
}

void AEQS_Helper::RemoveInsideActor(AActor* InActor)
{
	if (!ActorsInsideOnRegion.Contains(InActor))return;
	ActorsInsideOnRegion.Remove(InActor);

#if !UE_BUILD_SHIPPING
	if (bShowDebug)
	{
		UE_LOG(LogTemp, Log, TEXT("-----ACTOR REMOVED----"));
		UE_LOG(LogTemp, Log, TEXT("Actors Removed '%s'---> %d"), *RegionTag.ToString(), ActorsInsideOnRegion.Num());
	}
#endif
	
}

void AEQS_Helper::AddInsideActor(AActor* InActor)
{
	if (ActorsInsideOnRegion.Contains(InActor))return;
	ActorsInsideOnRegion.Add(InActor);

	if (ANPCBaseStateEnemy* BaseStateEnemy = Cast<ANPCBaseStateEnemy>(InActor))
	{
		BaseStateEnemy ->SetMyAreaTag(RegionTag);
	}
	
#if !UE_BUILD_SHIPPING
	if (bShowDebug)
	{
		UE_LOG(LogTemp, Log, TEXT("-----ACTOR ADDED----"));
		UE_LOG(LogTemp, Log, TEXT("Actors Added '%s'---> %d"), *RegionTag.ToString(), ActorsInsideOnRegion.Num());
	}
#endif
}

void AEQS_Helper::UpdateRegionEntity(AActor* InActor)
{
	if (ActorsInsideOnRegion.Num() == 0)return;
	
	for (auto Element : ActorsInsideOnRegion)
	{
		if (ANPCBaseStateEnemyController* EntityController = Cast<ANPCBaseStateEnemyController>(Element->GetInstigatorController()))
		{
			EntityController->SetStateAsAttacking(InActor);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Entity controller not found!"));
		}
	}
}


