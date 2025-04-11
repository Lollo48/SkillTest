// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "AI/NPC/NPCBase/NPCBase.h"
#include "SpawnPointBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FSpawnPointClear);

UCLASS()
class LGUEDK_API ASpawnPointBase : public AActor
{
	GENERATED_BODY()

public:
	
	ASpawnPointBase();
	
	UPROPERTY(BlueprintAssignable)
	FSpawnPointClear OnSpawnPointClear;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	UFUNCTION(Blueprintable,BlueprintCallable, Category = "Spawn")
	void SetIsClear(const bool bClear) {bIsClear = bClear;}

	UFUNCTION(Blueprintable,BlueprintCallable, Category = "Spawn")
	bool GetIsClear() const {return bIsClear;}
	
	UFUNCTION(Blueprintable,BlueprintCallable, Category = "Spawn")
	void SpawnEnemy(TSubclassOf<ANPCBase> const& EnemyClass);
	
	UFUNCTION(Blueprintable,BlueprintCallable, Category = "Spawn")
	int32 GetSpawnIndex() const {return Index;}
	
protected:
	
	UPROPERTY()
	TArray<AActor*> SpawnedEnemies;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	int32 Index;
	
private:

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float SpawnRadius;
	UPROPERTY(EditAnywhere, Category = "Spawn")
	float DistanceBetweenEnemies = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Spawn")
	bool bDrawDebug;

	TArray<FVector> PossibleSpawnPoints;
	
	UPROPERTY()
	bool bIsClear;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void OnEnemyDead(AActor* InAttackTarget);
	
	UFUNCTION(Blueprintable,BlueprintCallable, Category = "Spawn")
	void DrawDebug();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	TArray<FVector> GeneratePoints();
	
};
