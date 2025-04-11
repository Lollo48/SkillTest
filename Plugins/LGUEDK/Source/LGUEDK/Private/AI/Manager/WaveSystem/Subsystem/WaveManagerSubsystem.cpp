// Copyright Villains, Inc. All Rights Reserved.


#include "AI/Manager/WaveSystem/Subsystem/WaveManagerSubsystem.h"

#include <string>

#include "AI/Manager/WaveSystem/Utility/WaveManagerUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/LGDebug.h"


void UWaveManagerSubsystem::InitSpawnPoint(ASpawnPointBase* InSpawnPoint)
{
	if (!InSpawnPoint)
	{
		LGDebug::Log("SpawnPoint non valido", true);
		return;
	}

	int32 SpawnPointIndex = InSpawnPoint->GetSpawnIndex();

	if (!PossibleSpawnPoints.Contains(SpawnPointIndex))
	{
		PossibleSpawnPoints.Add(SpawnPointIndex, InSpawnPoint);
		InSpawnPoint->OnSpawnPointClear.AddDynamic(this, &UWaveManagerSubsystem::OnSpawnPointClear);
	}
	else
	{
		LGDebug::Log("SpawnPoint already existe", true);
	}
}

void UWaveManagerSubsystem::SetCurrentWavesByTag(FGameplayTag InWaveTag)
{
	if (Waves.Contains(InWaveTag))
	{
		CurrentWaves = Waves[InWaveTag].Waves;
		CurrentWaveIndex = 0;
	}
}

void UWaveManagerSubsystem::StartWave()
{
	OnStartNewWave.Broadcast(CurrentWave->TimeBeforeNextWave);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UWaveManagerSubsystem::SpawnWave, CurrentWave->TimeBeforeNextWave, false);
}

void UWaveManagerSubsystem::SpawnWave()
{
	for (const FEnemySpawnInfo& EnemyInfo : CurrentWave->EnemiesToSpawn)
	{
		int32 SpawnPointIndex = EnemyInfo.SpawnPointIndex;

		if (PossibleSpawnPoints.Contains(SpawnPointIndex))
		{
			if (ASpawnPointBase* SpawnPoint = PossibleSpawnPoints[SpawnPointIndex])
			{
				for (const FEnemyQuantityInfo& EnemyQuantityInfo : EnemyInfo.ListEnemyToSpawn)
				{
					const TArray<TSubclassOf<ANPCBase>>& EnemyClassArray = EnemyQuantityInfo.EnemyClasses;
					int Quantity = EnemyQuantityInfo.Quantity;
					
					for (const TSubclassOf<ANPCBase>& EnemyClass : EnemyClassArray)
					{
						for (int i = 0; i < Quantity; i++)
						{
							SpawnPoint->SpawnEnemy(EnemyClass);
							AliveEnemies ++;
						}
					}
				}
			}
		}
		else
		{
			LGDebug::Log("SpawnPointIndex not found in SpawnPointManager",true);
		}
	}
	
	//LGDebug::Log("Spawnati tutti i nemici", true);
	OnAllEntitySpawned.Broadcast(AliveEnemies);
	TimerHandle.Invalidate();
}

void UWaveManagerSubsystem::TryStartWave()
{
	if (CurrentWaves.Num() <= 0)
	{
		LGDebug::Log("CurrentWaves è vuoto!", true);
		return;
	}
	
	if (!CurrentWaves.IsValidIndex(CurrentWaveIndex))
	{
		LGDebug::Log(FString::Printf(TEXT("Indice %d fuori range! Max: %d"), CurrentWaveIndex, CurrentWaves.Num() - 1), true);
		return;
	}
	
	CurrentWave = CurrentWaves[CurrentWaveIndex];
	if (!CurrentWave)
	{
		LGDebug::Log("Wave corrente è nullptr!", true);
		return;
	}
	
	//LGDebug::Log("CurrentWaveIndex: " + FString::FromInt(CurrentWaveIndex), true);
	//LGDebug::Log("CurrentWaves.Num: " + FString::FromInt(CurrentWaves.Num()), true);
	
	if (CurrentWaveIndex < CurrentWaves.Num())
	{
		StartWave();
	}
	else
	{
		OnAllWaveClear.Broadcast();
		LGDebug::Log("WAVE FINITE COMPLIMENTI",true);
	}
}

void UWaveManagerSubsystem::EntityDead(AActor* InEntityDead)
{
	Super::EntityDead(InEntityDead);
	
	if (AliveEnemies <= 0)
	{
		OnLastEntityDead.Broadcast(InEntityDead);
		//LGDebug::Log("Ultimo nemico",true);
	}
}

void UWaveManagerSubsystem::BindToOnAllWaveClear(const FAllWaveClear& Context, bool bUnique)
{
	if (bUnique)
	{
		OnAllWaveClear.AddUnique(Context);
		return;
	}

	OnAllWaveClear.Add(Context);
}

void UWaveManagerSubsystem::UnBindToOnAllWaveClear(const FAllWaveClear& Context)
{
	OnAllWaveClear.Remove(Context);
}

void UWaveManagerSubsystem::BindToOnWaveClear(const FWaveClear& Context, bool bUnique)
{
	if (bUnique)
	{
		OnWaveClear.AddUnique(Context);
		return;
	}

	OnWaveClear.Add(Context);
}

void UWaveManagerSubsystem::UnBindToOnWaveClear(const FWaveClear& Context)
{
	OnWaveClear.Remove(Context);
}

void UWaveManagerSubsystem::BindToOnStartNewWave(const FstartNewWave& Context, bool bUnique)
{
	if (bUnique)
	{
		OnStartNewWave.AddUnique(Context);
		return;
	}

	OnStartNewWave.Add(Context);
}

void UWaveManagerSubsystem::UnBindToOnStartNewWave(const FstartNewWave& Context)
{
	OnStartNewWave.Remove(Context);
}

void UWaveManagerSubsystem::BindToOnAllEntitySpawned(const FAllEntitySpawned& Context, bool bUnique)
{
	if (bUnique)
	{
		OnAllEntitySpawned.AddUnique(Context);
		return;
	}
	
	OnAllEntitySpawned.Add(Context);
}

void UWaveManagerSubsystem::UnBindToOnAllEntitySpawned(const FAllEntitySpawned& Context)
{
	OnAllEntitySpawned.Remove(Context);
}

void UWaveManagerSubsystem::BindToOnLastEntityDead(const FLastEntityDead& Context, bool bUnique)
{
	if (bUnique)
	{
		OnLastEntityDead.AddUnique(Context);
		return;
	}

	OnLastEntityDead.Add(Context);
}

void UWaveManagerSubsystem::UnBindToOnLastEntityDead(const FLastEntityDead& Context)
{
	OnLastEntityDead.Remove(Context);
}

void UWaveManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UWaveManagerUtility::InitWaveManager(this);
}

void UWaveManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UWaveManagerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	Init();
}

void UWaveManagerSubsystem::Init()
{
	PossibleSpawnPoints = TMap<int32, ASpawnPointBase*>();
	CurrentWaves = TArray<UWaveDataAsset*>();
	CurrentWave = nullptr;
	AliveEnemies = 0;
	//LGDebug::Log("Init UWaveManagerSubsystem", true);
	InitBP();
}

void UWaveManagerSubsystem::OnSpawnPointClear()
{
	for (int i = 0; i < PossibleSpawnPoints.Num(); i++)
	{
		if (!PossibleSpawnPoints[i]->GetIsClear()) 
		{
			return;
		}
	}
	
	OnWaveClear.Broadcast();
	
	//LGDebug::Log("All SpawnPoint Clear", true);
}
