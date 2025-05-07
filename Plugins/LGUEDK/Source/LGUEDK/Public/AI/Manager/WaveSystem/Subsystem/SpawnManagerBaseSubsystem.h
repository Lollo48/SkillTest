// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AI/Manager/WaveSystem/SpawnPointBase.h"
#include "Subsystems/WorldSubsystem.h"
#include "SpawnManagerBaseSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FSpawnEntityDispatcher , AActor* ,SpawnedEntity);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FEntityDeadDispatcher , AActor*, DeadActor, int32 , AliveEnemies);

DECLARE_DYNAMIC_DELEGATE_OneParam(FSpawnEntity, AActor* ,SpawnedEntity);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FEntityDead, AActor*, DeadActor, int32 , AliveEnemies);

/**
 * 
 */
UCLASS(Abstract,Blueprintable,BlueprintType)
class LGUEDK_API USpawnManagerBaseSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintAssignable)
	FSpawnEntityDispatcher OnSpawnedEntity;

	UPROPERTY(BlueprintAssignable)
	FEntityDeadDispatcher OnEntityDead;

	UFUNCTION()
	int32 GetAliveEnemies() const { return AliveEnemies; }

	UFUNCTION()
	virtual AActor* SpawnEntity(TSubclassOf<AActor> ActorToSpawn, FVector const SpawnLocation);

	UFUNCTION()
	virtual void EntityDead(AActor* InEntityDead);
	
	UFUNCTION(Category = "Trigger")
	virtual void BindToOnSpawnEntity(const FSpawnEntity& Context,bool bUnique = true);

	UFUNCTION(Category = "Trigger")
	virtual void UnBindToOnSpawnEntity(const FSpawnEntity& Context);
	
	UFUNCTION(Category = "Trigger")
	virtual void BindToOnEntityDead(const FEntityDead& Context,bool bUnique = true);
	
	UFUNCTION(Category = "Trigger")
	virtual void UnBindToOnEntityDead(const FEntityDead& Context);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Trigger")
	TArray<TSubclassOf<AGameModeBase>>TriggerManager;
	
	int32 AliveEnemies = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bShowDebug;

	UPROPERTY()
	UWorld* World;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;
	
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	UFUNCTION()
	virtual void Init();
	UFUNCTION(BlueprintImplementableEvent,Category="Spawn Manager Base")
	void InitBP();
	
	UFUNCTION(BlueprintImplementableEvent,Category="Spawn Manager Base")
	void SpawnEntityBP(TSubclassOf<AActor> ActorToSpawn, FVector const SpawnLocation);
	
	UFUNCTION(BlueprintImplementableEvent,Category="Spawn Manager Base")
	void EntityDeadBP(AActor* InEntityDead);


};
