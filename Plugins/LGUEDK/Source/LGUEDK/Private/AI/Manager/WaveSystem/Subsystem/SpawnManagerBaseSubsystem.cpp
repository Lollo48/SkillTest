// Copyright Villains, Inc. All Rights Reserved.


#include "AI/Manager/WaveSystem/Subsystem/SpawnManagerBaseSubsystem.h"

#include "AI/Manager/WaveSystem/Utility/SpawnManagerBaseUtility.h"
#include "GameFramework/GameModeBase.h"
#include "Utility/LGDebug.h"

AActor* USpawnManagerBaseSubsystem::SpawnEntity(TSubclassOf<AActor> ActorToSpawn, FVector const SpawnLocation)
{
	if (!ActorToSpawn) return nullptr;

	AActor* SpawnedEnemy = World->SpawnActor<AActor>(
		ActorToSpawn,
		SpawnLocation,
		FRotator::ZeroRotator
	);
	
	SpawnEntityBP(ActorToSpawn, SpawnLocation);
	
	AliveEnemies++;
	
	OnSpawnedEntity.Broadcast(SpawnedEnemy);
	
	return SpawnedEnemy;
}

void USpawnManagerBaseSubsystem::EntityDead(AActor* InEntityDead)
{
	AliveEnemies--;
	OnEntityDead.Broadcast(InEntityDead,AliveEnemies);
	EntityDeadBP(InEntityDead);
}

void USpawnManagerBaseSubsystem::BindToOnSpawnEntity(const FSpawnEntity& Context,
	bool bUnique)
{
	if (bUnique)
	{
		OnSpawnedEntity.AddUnique(Context);
		return;
	}

	OnSpawnedEntity.Add(Context);
}

void USpawnManagerBaseSubsystem::UnBindToOnSpawnEntity(const FSpawnEntity& Context)
{
	OnSpawnedEntity.Remove(Context);
}

void USpawnManagerBaseSubsystem::BindToOnEntityDead(const FEntityDead& Context, bool bUnique)
{
	if (bUnique)
	{
		OnEntityDead.AddUnique(Context);
		return;
	}

	OnEntityDead.Add(Context);
}

void USpawnManagerBaseSubsystem::UnBindToOnEntityDead(const FEntityDead& Context)
{
	OnEntityDead.Remove(Context);
}

void USpawnManagerBaseSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	USpawnManagerBaseUtility::Init(this);
}

void USpawnManagerBaseSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void USpawnManagerBaseSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	World = GetWorld();

	if (!IsValid(World))
	{
		LGDebug::Error("Inizialize failed because World is not valid", true);
		return;
	}
	
	const AGameModeBase* GameMode = World->GetAuthGameMode();
	
	if (!IsValid(GameMode))
	{
		LGDebug::Error("Inizialize failed because GameMode is not valid", true);
		return;
	}
	
	if (!TriggerManager.Contains(GameMode->GetClass()))
	{
		if (bShowDebug)
			LGDebug::Error("GameMode doesn't exist", true);
		return;
	}
	
	if (bShowDebug)
		LGDebug::Log("GameMode corretta", true);

	
}

void USpawnManagerBaseSubsystem::Init()
{
	
	InitBP();
}


