// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "AI/NPC/NPCBase/NPCBase.h"
#include "SpawnPointBase.generated.h"

UCLASS()
class LGUEDK_API ASpawnPointBase : public AActor
{
	GENERATED_BODY()

public:
	
	ASpawnPointBase();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;
	
	UFUNCTION(Blueprintable,BlueprintCallable, Category = "Spawn")
	virtual void SpawnEnemy(TSubclassOf<ANPCBase> const& EnemyClass);
	
	UFUNCTION(Blueprintable,BlueprintCallable, Category = "Spawn")
	int32 GetSpawnIndex() const {return Index;}
	
protected:

	UPROPERTY(EditAnywhere, Category = "Spawn")
	int32 Index;

	TArray<FVector> PossibleSpawnPoints;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void GeneratePoints();
	
private:

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float SpawnRadius;
	UPROPERTY(EditAnywhere, Category = "Spawn")
	float DistanceBetweenEnemies = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Spawn")
	bool bDrawDebug;
	
	UFUNCTION(Blueprintable,BlueprintCallable, Category = "Spawn")
	void DrawDebug();
	
};
