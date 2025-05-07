// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/Manager/WaveSystem/Subsystem/SpawnManagerBaseSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpawnManagerBaseUtility.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API USpawnManagerBaseUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static USpawnManagerBaseSubsystem* SpawnManagerSubsystem;

	static void Init(USpawnManagerBaseSubsystem* InSpawnManagerSubsystem);
	
	UFUNCTION(BlueprintCallable)
	static USpawnManagerBaseSubsystem* GetSpawnManagerSubsystem(){ return SpawnManagerSubsystem; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static int32 GetAliveEnemies();

	UFUNCTION(BlueprintCallable)
	static AActor* SpawnEntity(TSubclassOf<AActor> ActorToSpawn, FVector const SpawnLocation);

	UFUNCTION(BlueprintCallable)
	static void EntityDead(AActor* InEntityDead);
	
	UFUNCTION(BlueprintCallable)
	static void BindToOnSpawnEntity(const FSpawnEntity& Context,bool bUnique = true);

	UFUNCTION(BlueprintCallable)
	static void UnBindToOnSpawnEntity(const FSpawnEntity& Context);
	
	UFUNCTION(BlueprintCallable)
	static void BindToOnEntityDead(const FEntityDead& Context,bool bUnique = true);

	UFUNCTION(BlueprintCallable)
	static void UnBindToOnEntityDead(const FEntityDead& Context);
};
