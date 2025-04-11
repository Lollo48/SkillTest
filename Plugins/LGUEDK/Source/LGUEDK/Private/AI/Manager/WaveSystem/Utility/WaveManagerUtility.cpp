// Copyright Villains, Inc. All Rights Reserved.


#include "AI/Manager/WaveSystem/Utility/WaveManagerUtility.h"

#include "Utility/LGDebug.h"

UWaveManagerSubsystem* UWaveManagerUtility::WaveManagerSubsystem = nullptr;

void UWaveManagerUtility::InitWaveManager(UWaveManagerSubsystem* InWaveManagerSubsystem)
{
	if (!IsValid(InWaveManagerSubsystem))
	{
		LGDebug::Error("WaveManagerSubsystem is not valid", true);
		return;
	}
	
	WaveManagerSubsystem = InWaveManagerSubsystem;

}

void UWaveManagerUtility::AddPointToSpawnPointManager(ASpawnPointBase* InSpawnPoint)
{
	
}

int32 UWaveManagerUtility::GetWaveIndex()
{
	return  WaveManagerSubsystem->GetWaveIndex();
}

void UWaveManagerUtility::IncreaseCurrentWaveIndex(int32 Increase)
{
	WaveManagerSubsystem->IncreaseCurrentWaveIndex(Increase);
}

void UWaveManagerUtility::InitSpawnPoint(ASpawnPointBase* InSpawnPoint)
{
	WaveManagerSubsystem->InitSpawnPoint(InSpawnPoint);
}

void UWaveManagerUtility::SetCurrentWavesByTag(FGameplayTag InWaveTag)
{
	WaveManagerSubsystem->SetCurrentWavesByTag(InWaveTag);
}

void UWaveManagerUtility::TryStartWave()
{
	WaveManagerSubsystem->TryStartWave();
}

void UWaveManagerUtility::EntityDead(AActor* InEntityDead)
{
	WaveManagerSubsystem->EntityDead(InEntityDead);
}

void UWaveManagerUtility::BindToOnSpawnEntity(const FSpawnEntity& Context, bool bUnique)
{
	WaveManagerSubsystem->BindToOnSpawnEntity(Context, bUnique);
}

void UWaveManagerUtility::UnBindToOnSpawnEntity(const FSpawnEntity& Context)
{
	WaveManagerSubsystem->UnBindToOnSpawnEntity(Context);
}

void UWaveManagerUtility::BindToOnEntityDead(const FEntityDead& Context, bool bUnique)
{
	WaveManagerSubsystem->BindToOnEntityDead(Context, bUnique);
}

void UWaveManagerUtility::UnBindToOnEntityDead(const FEntityDead& Context)
{
	WaveManagerSubsystem->UnBindToOnEntityDead(Context);
}

void UWaveManagerUtility::BindToOnAllWaveClear(const FAllWaveClear& Context, bool bUnique)
{
	WaveManagerSubsystem->BindToOnAllWaveClear(Context, bUnique);
}

void UWaveManagerUtility::UnBindToOnAllWaveClear(const FAllWaveClear& Context)
{
	WaveManagerSubsystem->UnBindToOnAllWaveClear(Context);
}

void UWaveManagerUtility::BindToOnWaveClear(const FWaveClear& Context, bool bUnique)
{
	WaveManagerSubsystem->BindToOnWaveClear(Context, bUnique);
}

void UWaveManagerUtility::UnBindToOnWaveClear(const FWaveClear& Context)
{
	WaveManagerSubsystem->UnBindToOnWaveClear(Context);
}

void UWaveManagerUtility::BindToOnStartNewWave(const FstartNewWave& Context, bool bUnique)
{
	WaveManagerSubsystem->BindToOnStartNewWave(Context, bUnique);
}

void UWaveManagerUtility::UnBindToOnStartNewWave(const FstartNewWave& Context)
{
	WaveManagerSubsystem->UnBindToOnStartNewWave(Context);
}

void UWaveManagerUtility::BindToOnAllEntitySpawned(const FAllEntitySpawned& Context, bool bUnique)
{
	WaveManagerSubsystem->BindToOnAllEntitySpawned(Context, bUnique);
}

void UWaveManagerUtility::UnBindToOnAllEntitySpawned(const FAllEntitySpawned& Context)
{
	WaveManagerSubsystem->UnBindToOnAllEntitySpawned(Context);
}

void UWaveManagerUtility::BindToOnLastEntityDead(const FLastEntityDead& Context, bool bUnique)
{
	WaveManagerSubsystem->BindToOnLastEntityDead(Context, bUnique);
}

void UWaveManagerUtility::UnBindToOnLastEntityDead(const FLastEntityDead& Context)
{
	WaveManagerSubsystem->UnBindToOnLastEntityDead(Context);
}


     