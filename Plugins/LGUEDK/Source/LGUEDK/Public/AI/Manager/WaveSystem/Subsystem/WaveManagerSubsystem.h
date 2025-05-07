// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SpawnManagerBaseSubsystem.h"
#include "AI/Manager/WaveSystem/Data/WaveDataAsset.h"
#include "AI/Manager/WaveSystem/Data/WaveQuantity.h"
#include "WaveManagerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FAllWaveClearDispatcher);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FWaveClearDispatcher);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FStartNewWaveDispatcher, float, Time,int , WaveIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEndWaveDispatcher,int , WaveIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FAllEntitySpawnedDispatcher, int, CountEnemy);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FLastEntityDeadDispatcher, AActor*, DeadActor);

DECLARE_DYNAMIC_DELEGATE(FAllWaveClear);

DECLARE_DYNAMIC_DELEGATE(FWaveClear);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FstartNewWave, float, Time,int , WaveIndex);

DECLARE_DYNAMIC_DELEGATE_OneParam(FAllEntitySpawned, int, CountEnemy);

DECLARE_DYNAMIC_DELEGATE_OneParam(FLastEntityDead, AActor*, DeadActor);

DECLARE_DYNAMIC_DELEGATE_OneParam(FEndWave,int , WaveIndex);


/**
 * 
 */
UCLASS(Abstract,Blueprintable,BlueprintType)
class LGUEDK_API UWaveManagerSubsystem : public USpawnManagerBaseSubsystem
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FAllWaveClearDispatcher OnAllWaveClear;

	UPROPERTY(BlueprintAssignable)
	FWaveClearDispatcher OnWaveClear;
 
	UPROPERTY(BlueprintAssignable)
	FStartNewWaveDispatcher OnStartNewWave;

	UPROPERTY(BlueprintAssignable)
	FEndWaveDispatcher OnEndWave;

	UPROPERTY(BlueprintAssignable)
	FAllEntitySpawnedDispatcher OnAllEntitySpawned;

	UPROPERTY(BlueprintAssignable)
	FLastEntityDeadDispatcher OnLastEntityDead;
	
	UFUNCTION()
	int32 GetWaveIndex() const {return CurrentWaveIndex;}

	UFUNCTION()
	void IncreaseCurrentWaveIndex(int32 Increase) {CurrentWaveIndex += Increase;}

	UFUNCTION()
	void InitSpawnPoint(ASpawnPointBase* InSpawnPoint);

	UFUNCTION()
	void SetCurrentWavesByTag(FGameplayTag InWaveTag);
	
	UFUNCTION()
	virtual void TryStartWave();
	
	void EntityDead(AActor* InEntityDead) override;

	virtual void BindToOnSpawnEntity(const FSpawnEntity& Context, bool bUnique = true) override{Super::BindToOnSpawnEntity(Context, bUnique);}

	virtual void UnBindToOnSpawnEntity(const FSpawnEntity& Context) override{Super::UnBindToOnSpawnEntity(Context);}

	virtual void BindToOnEntityDead(const FEntityDead& Context, bool bUnique = true) override{Super::BindToOnEntityDead(Context, bUnique);}

	virtual void UnBindToOnEntityDead(const FEntityDead& Context) override{Super::UnBindToOnEntityDead(Context);}
	
	UFUNCTION(Category = "Wave Manager")
	void BindToOnAllWaveClear(const FAllWaveClear& Context,bool bUnique = true);

	UFUNCTION(Category = "Wave Manager")
	void UnBindToOnAllWaveClear(const FAllWaveClear& Context);

	UFUNCTION(Category = "Wave Manager")
	void BindToOnWaveClear(const FWaveClear& Context,bool bUnique = true);
	
	UFUNCTION(Category = "Wave Manager")
	void UnBindToOnWaveClear(const FWaveClear& Context);
	
	UFUNCTION(Category = "Wave Manager")
	void BindToOnStartNewWave(const FstartNewWave& Context,bool bUnique = true);
	
	UFUNCTION(Category = "Wave Manager")
	void UnBindToOnStartNewWave(const FstartNewWave& Context);

	UFUNCTION(Category = "Wave Manager")
	void BindToOnEndWave(const FEndWave& Context,bool bUnique = true);

	UFUNCTION(Category = "Wave Manager")
	void UnBindToOnEndWave(const FEndWave& Context);
	
	UFUNCTION(Category = "Wave Manager")
	void BindToOnAllEntitySpawned(const FAllEntitySpawned& Context,bool bUnique = true);
	
	UFUNCTION(Category = "Wave Manager")
	void UnBindToOnAllEntitySpawned(const FAllEntitySpawned& Context);

	UFUNCTION(Category = "Wave Manager")
	void BindToOnLastEntityDead(const FLastEntityDead& Context,bool bUnique = true);
	
	UFUNCTION(Category = "Wave Manager")
	void UnBindToOnLastEntityDead(const FLastEntityDead& Context);

protected:
	
	UPROPERTY(EditAnywhere, Category = "Wave System")
	TMap<FGameplayTag, FWaveQuantity> Waves;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual void Init() override;

private:
	
	FTimerHandle TimerHandle;

	UPROPERTY()
	TArray<UWaveDataAsset*> CurrentWaves;
	
	UPROPERTY()
	TMap<int32,ASpawnPointBase*> PossibleSpawnPoints;
	
	int32 CurrentWaveIndex = 0;

	UPROPERTY()
	UWaveDataAsset* CurrentWave;

	UFUNCTION()
	virtual void StartWave();
	
	UFUNCTION()
	virtual void SpawnWave();
	
};
