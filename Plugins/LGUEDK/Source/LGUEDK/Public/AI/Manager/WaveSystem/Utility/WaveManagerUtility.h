// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/Manager/WaveSystem/SpawnPointBase.h"
#include "AI/Manager/WaveSystem/Subsystem/WaveManagerSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WaveManagerUtility.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UWaveManagerUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static UWaveManagerSubsystem* WaveManagerSubsystem;
	
	static void InitWaveManager(UWaveManagerSubsystem* InWaveManagerSubsystem);

	static void AddPointToSpawnPointManager(ASpawnPointBase* InSpawnPoint);

	UFUNCTION(BlueprintCallable,BlueprintPure)
	static int32 GetWaveIndex();

	UFUNCTION(BlueprintCallable)
	static void IncreaseCurrentWaveIndex(int32 Increase);

	UFUNCTION(BlueprintCallable)
	static void InitSpawnPoint(ASpawnPointBase* InSpawnPoint);
	
	UFUNCTION(BlueprintCallable)
	static void SetCurrentWavesByTag(FGameplayTag InWaveTag);
	
	UFUNCTION(BlueprintCallable)
	static void TryStartWave();
	
	static void EntityDead(AActor* InEntityDead);
	
	UFUNCTION(BlueprintCallable,  Category = "Wave Manager")
	static void BindToOnSpawnEntity(const FSpawnEntity& Context,bool bUnique = true);

	UFUNCTION(BlueprintCallable,  Category = "Wave Manager")
	static void UnBindToOnSpawnEntity(const FSpawnEntity& Context);
	
	UFUNCTION(BlueprintCallable,  Category = "Wave Manager")
	static void BindToOnEntityDead(const FEntityDead& Context,bool bUnique = true);
	
	UFUNCTION(BlueprintCallable,  Category = "Wave Manager")
	static void UnBindToOnEntityDead(const FEntityDead& Context);
	
	UFUNCTION(BlueprintCallable,  Category = "Wave Manager")
	static void BindToOnAllWaveClear(const FAllWaveClear& Context,bool bUnique = true);

	UFUNCTION(BlueprintCallable,  Category = "Wave Manager")
	static void UnBindToOnAllWaveClear(const FAllWaveClear& Context);

	UFUNCTION(BlueprintCallable,  Category = "Wave Manager")
	static void BindToOnWaveClear(const FWaveClear& Context,bool bUnique = true);
	
	UFUNCTION(BlueprintCallable,  Category = "Wave Manager")
	static void UnBindToOnWaveClear(const FWaveClear& Context);
	
	UFUNCTION(BlueprintCallable,  Category = "Wave Manager")
	static void BindToOnStartNewWave(const FstartNewWave& Context,bool bUnique = true);
	
	UFUNCTION(BlueprintCallable,  Category = "Wave Manager")
	static void UnBindToOnStartNewWave(const FstartNewWave& Context);
	
	UFUNCTION(BlueprintCallable,  Category = "Wave Manager")
	static void BindToOnAllEntitySpawned(const FAllEntitySpawned& Context,bool bUnique = true);
	
	UFUNCTION(BlueprintCallable,  Category = "Wave Manager")
	static void UnBindToOnAllEntitySpawned(const FAllEntitySpawned& Context);

	UFUNCTION(BlueprintCallable,  Category = "Wave Manager")
	void BindToOnLastEntityDead(const FLastEntityDead& Context,bool bUnique = true);
	
	UFUNCTION(BlueprintCallable,  Category = "Wave Manager")
	void UnBindToOnLastEntityDead(const FLastEntityDead& Context);
};
