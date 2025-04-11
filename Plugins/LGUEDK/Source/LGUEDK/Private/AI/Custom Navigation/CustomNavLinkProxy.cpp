// Copyright Villains, Inc. All Rights Reserved.


#include "AI/Custom Navigation/CustomNavLinkProxy.h"

#include "AIController.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
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

	JumpHeightRequired = FMath::Abs(LeftPt.Z - RightPt.Z);
}

bool ACustomNavLinkProxy::IsAvailable() const
{
	return !bIsOccupied;
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

	UE_LOG(LogTemp, Log, TEXT("Nav Link marked as occupied for %.2f seconds"), OccupyDuration);
}

FVector ACustomNavLinkProxy::FindValidDestination(const FVector& OriginalDestination,const int32& MaxAttempts,bool& bDebug)
{
	FVector CurrentDestination = OriginalDestination;
	FVector Direction = (CurrentDestination - GetActorLocation()).GetSafeNormal();
	float const Radius = 100.0f;
		
	for (int32 Attempt = 0; Attempt < MaxAttempts; ++Attempt)
	{
		FHitResult HitResult;
		bool bHit = GetWorld()->SweepSingleByChannel(HitResult,GetActorLocation(),CurrentDestination,FQuat::Identity,ECC_Visibility,FCollisionShape::MakeSphere(Radius));

		if (bDebug)
		{
			// Disegna il percorso dello sphere cast
			DrawDebugSphere(GetWorld(), CurrentDestination, Radius, 12, FColor::Red, false, 2.0f);
			DrawDebugLine(GetWorld(), GetActorLocation(), CurrentDestination, bHit ? FColor::Red : FColor::Green, false, 2.0f);
		}

		if (!bHit)
		{
			// Nessun ostacolo, ritorna un punto casuale entro il raggio dello sphere cast
			FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(
				CurrentDestination,
				FVector(Radius, Radius, Radius)
			);

			if (bDebug)
			{
				DrawDebugSphere(GetWorld(), RandomPoint, 10.0f, 8, FColor::Green, false, 2.0f);
				UE_LOG(LogTemp, Log, TEXT("Valid destination found: %s"), *RandomPoint.ToString());
			}

			return RandomPoint;
		}

		// Se c'è un ostacolo, sposta a destra o a sinistra di 100 unità
		FVector RightVector = FVector::CrossProduct(Direction, FVector::UpVector).GetSafeNormal();
		CurrentDestination += (Attempt % 2 == 0 ? RightVector : -RightVector) * 100.0f;

		if (bDebug)
		{
			UE_LOG(LogTemp, Warning, TEXT("Obstacle detected. Adjusting destination to: %s"), *CurrentDestination.ToString());
		}
	}

	// Ritorna la destinazione originale se non si trova un percorso valido
	if (bDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find a valid destination after %d attempts"), MaxAttempts);
	}

	return OriginalDestination;
}

bool ACustomNavLinkProxy::CheckJumpHeightRequired(AActor* Pawn) const
{
	ANPCBaseStateEnemy* NPC = Cast<ANPCBaseStateEnemy>(Pawn);
	if (!NPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn is not an NPC"));
		return false;
	}
	
	return NPC->GetDataAsset()->MaxJumpingHeight > JumpHeightRequired;
}

void ACustomNavLinkProxy::Release()
{
	bIsOccupied = false;
	
	GetWorldTimerManager().ClearTimer(OccupyTimerHandle);

	UE_LOG(LogTemp, Log, TEXT("Nav Link is now available again"));
}
