// Copyright Villains, Inc. All Rights Reserved.


#include "AI/Custom Navigation/CustomNavLinkProxy.h"

#include "AIController.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utility/LGDebug.h"


// Sets default values
ACustomNavLinkProxy::ACustomNavLinkProxy()
{
	PrimaryActorTick.bCanEverTick = true;
	bSmartLinkIsRelevant = true;
	bIsOccupied = false;
	
}

void ACustomNavLinkProxy::BeginPlay()
{
	Super::BeginPlay();
	OccupyTimerHandle.Invalidate();
	
	UNavLinkCustomComponent* NavLinkCustomComponent =  GetSmartLinkComp();
	FVector LeftPt = FVector(70, 0, 0);
	FVector RightPt = FVector(-70, 0, 0);
	ENavLinkDirection::Type Direction;
	NavLinkCustomComponent->GetLinkData(LeftPt, RightPt, Direction);
	Direction = AINavLinkDirection;
	NavLinkCustomComponent->SetLinkData(LeftPt, RightPt, Direction);
	
}

bool ACustomNavLinkProxy::IsAvailable() const
{
	return bIsOccupied;
}

void ACustomNavLinkProxy::MarkAsOccupied(float OccupyDuration)
{
	if (bIsOccupied)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nav Link is already occupied!"));
		return;
	}
	
	bIsOccupied = true;
	
	GetWorldTimerManager().SetTimer(
		OccupyTimerHandle, 
		this, 
		&ACustomNavLinkProxy::Release, 
		OccupyDuration, 
		false
	);
}

bool ACustomNavLinkProxy::FindValidDestination(const FVector& OriginalDestination)
{
	if (IsPointFree(OriginalDestination))return true;

	return false;
}

void ACustomNavLinkProxy::EnableCollider(AActor* ActorToEnableCollider)
{
	if (ANPCBase* NPCBase = Cast<ANPCBase>(ActorToEnableCollider))
	{
		NPCBase->ResetEntityCollision();
	}
}

bool ACustomNavLinkProxy::IsPointFree(const FVector& Point) const
{
	FCollisionQueryParams QueryParams;

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
	
	return !bHit;
}

void ACustomNavLinkProxy::Release()
{
	bIsOccupied = false;
	GetWorldTimerManager().ClearTimer(OccupyTimerHandle);
	
}
