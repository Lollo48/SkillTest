// Copyright Villains, Inc. All Rights Reserved.


#include "AI/Manager/WaveSystem/SpawnPointBase.h"

#include "AI/EQS/EQSUtility.h"
#include "AI/Manager/WaveSystem/Utility/WaveManagerUtility.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/LGDebug.h"

ASpawnPointBase::ASpawnPointBase()
{
	bIsClear = true;
}


void ASpawnPointBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (bDrawDebug)
		DrawDebug();
}

void ASpawnPointBase::BeginPlay()
{
	Super::BeginPlay();
	UWaveManagerUtility::InitSpawnPoint(this);
}

void ASpawnPointBase::SpawnEnemy(TSubclassOf<ANPCBase>const& EnemyClass) 
{
	if (!EnemyClass)return;

	if (PossibleSpawnPoints.Num() == 0)
		PossibleSpawnPoints = GeneratePoints();
	
	int32 InIndex = FMath::RandRange(0, PossibleSpawnPoints.Num() - 1);
	FVector SpawnPoint = PossibleSpawnPoints[InIndex];
	SpawnPoint.Z += 50.f;
	PossibleSpawnPoints.Remove(SpawnPoint);
	
	const FRotator SpawnRotation = FRotator::ZeroRotator;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	if (ANPCBase* SpawnedEnemy = GetWorld()->SpawnActor<ANPCBase>(EnemyClass, SpawnPoint, SpawnRotation,SpawnParams))
	{
		SpawnedEnemies.Add(SpawnedEnemy);

		//LGDebug::Log("Nemico spawnato", true);
		
#if UE_EDITOR
		const FString FolderName = TEXT("SpawnedEnemies/SpawnPoint");
		SpawnedEnemy->SetFolderPath(*FolderName);
#endif
				
		SetIsClear(false);

		if (ANPCBaseStateEnemy* EnemyState = Cast<ANPCBaseStateEnemy>(SpawnedEnemy))
		{
			EnemyState->OnStateDead.AddDynamic(this, &ASpawnPointBase::OnEnemyDead);
		}
	}
	
	//LGDebug::Log(FString::Printf(TEXT("%d"), SpawnedEnemies.Num()), true);
}

void ASpawnPointBase::DrawDebug()
{
	DrawDebugSphere(GetWorld(), GetActorLocation(), SpawnRadius, 12, FColor::Green, false, 0.01f);
}

TArray<FVector> ASpawnPointBase::GeneratePoints()
{
	FVector SpawnLocation = GetActorLocation();
	TArray<FVector> CurrentSpawnPoint;
	return CurrentSpawnPoint = UEQSUtility::GenerateGridPoints(SpawnLocation,SpawnRadius,DistanceBetweenEnemies);
}

void ASpawnPointBase::OnEnemyDead(AActor* InAttackTarget)
{
	if (SpawnedEnemies.Contains(InAttackTarget))
	{
		SpawnedEnemies.Remove(InAttackTarget);
	}
	
	//LGDebug::Log(FString::Printf(TEXT("%d"), SpawnedEnemies.Num()), true);
	
	if (SpawnedEnemies.Num() == 0)
	{
		//dico al manager che ho finito i miei nemici
		SetIsClear(true);
		OnSpawnPointClear.Broadcast();
		//LGDebug::Log("SpawnPoint  is clear", true);
	}
}


