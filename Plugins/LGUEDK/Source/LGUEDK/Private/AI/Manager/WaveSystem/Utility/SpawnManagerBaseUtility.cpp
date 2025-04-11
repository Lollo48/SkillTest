// Copyright Villains, Inc. All Rights Reserved.


#include "AI/Manager/WaveSystem/Utility/SpawnManagerBaseUtility.h"

#include "Utility/LGDebug.h"

USpawnManagerBaseSubsystem* USpawnManagerBaseUtility::SpawnManagerSubsystem = nullptr;

void USpawnManagerBaseUtility::Init(USpawnManagerBaseSubsystem* InSpawnManagerSubsystem)
{
	if (!IsValid(InSpawnManagerSubsystem))
	{
		LGDebug::Error("TriggerSubsystem is not valid", true);
		return;
	}
	
	SpawnManagerSubsystem = InSpawnManagerSubsystem;
}

int32 USpawnManagerBaseUtility::GetAliveEnemies()
{
	return SpawnManagerSubsystem->GetAliveEnemies();
}

void USpawnManagerBaseUtility::SetAliveEnemies(int32 const InAliveEnemies)
{
	SpawnManagerSubsystem->SetAliveEnemies(InAliveEnemies);
}

AActor* USpawnManagerBaseUtility::SpawnEntity(TSubclassOf<AActor> ActorToSpawn, FVector const SpawnLocation)
{
	return SpawnManagerSubsystem->SpawnEntity(ActorToSpawn, SpawnLocation);
}

void USpawnManagerBaseUtility::EntityDead(AActor* InEntityDead)
{
	SpawnManagerSubsystem->EntityDead(InEntityDead);
}

void USpawnManagerBaseUtility::BindToOnSpawnEntity(const FSpawnEntity& Context, bool bUnique)
{
	SpawnManagerSubsystem->BindToOnSpawnEntity(Context, bUnique);
}

void USpawnManagerBaseUtility::UnBindToOnSpawnEntity(const FSpawnEntity& Context)
{
	SpawnManagerSubsystem->UnBindToOnSpawnEntity(Context);
}

void USpawnManagerBaseUtility::BindToOnEntityDead(const FEntityDead& Context, bool bUnique)
{
	SpawnManagerSubsystem->BindToOnEntityDead(Context, bUnique);
}

void USpawnManagerBaseUtility::UnBindToOnEntityDead(const FEntityDead& Context)
{
	SpawnManagerSubsystem->UnBindToOnEntityDead(Context);
}
