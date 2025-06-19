// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "Data/TeamData.h"
#include "GameplayTagContainer.h"
#include "Data/MaxEnemiesPerTypeData.h"
#include "Data/AttackingTeam.h"
#include "Subsystems/WorldSubsystem.h"
#include "FlowAttacksManager.generated.h"

/**
 * 
 */
UCLASS(Abstract,Blueprintable,BlueprintType)
class LGUEDK_API UFlowAttacksManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	void RegisterEntity(ANPCBaseStateEnemy* Requester,const EEnemyType EnemyType, const FGameplayTag MyTeam);   
	void UnregisterEntity(ANPCBaseStateEnemy* Requester,const EEnemyType EnemyType, const FGameplayTag MyTeam);                 
	void UpdateActiveEntity(const EEnemyType EnemyType, const FGameplayTag MyTeam);                              
	void ForceRegisterEntity(ANPCBaseStateEnemy* Requester,const EEnemyType EnemyType, const FGameplayTag MyTeam, const AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "FlowGame|Debug")
	void DebugEntityMaps() const;
	
protected:

	UPROPERTY()
	bool bIsInitialized = false;
	
	UPROPERTY(EditAnywhere, Category = "Attack Settings")
	bool bShowDebug;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

	virtual void Init();

	UFUNCTION(BlueprintImplementableEvent,Category="Triggerbehavior")
	void InitBP();
	
	UFUNCTION()
	virtual void SetPendingEntity(ANPCBaseStateEnemy* Requester);
	UFUNCTION()
	virtual void SetActiveEntity(ANPCBaseStateEnemy* Requester);

private:

	UPROPERTY(EditDefaultsOnly, Category = "GameModes")
	TMap<TSubclassOf<AGameModeBase>,FAttackingTeam> GameModes;
	
	TMap<FGameplayTag, TMap<TEnumAsByte<EEnemyType>, int32>> EntitiesLimits;
	TMap<FGameplayTag, TMap<TEnumAsByte<EEnemyType>, TArray<ANPCBaseStateEnemy*>>> ActiveEntities;
	TMap<FGameplayTag, TMap<TEnumAsByte<EEnemyType>, TArray<ANPCBaseStateEnemy*>>> PendingEntities;

	UFUNCTION()
	bool IsValidMap(const EEnemyType EnemyType, const FGameplayTag MyTeam);
};
