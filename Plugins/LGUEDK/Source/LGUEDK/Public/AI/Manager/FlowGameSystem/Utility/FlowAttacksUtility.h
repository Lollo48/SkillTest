// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AI/Manager/FlowGameSystem/FlowAttacksManager.h"
#include "FlowAttacksUtility.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UFlowAttacksUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	static UFlowAttacksManager* FlowGameWorldSubSystem;

	static void Init(UFlowAttacksManager* InFlowGameWorldSubSystem);

	UFUNCTION(BlueprintCallable,BlueprintPure)
	static UFlowAttacksManager* GetTriggerBaseSubsystem(){return FlowGameWorldSubSystem;}
	
	UFUNCTION(BlueprintCallable)
	static void RegisterEntity(ANPCBaseStateEnemy* Requester,const EEnemyType EnemyType, const FGameplayTag MyTeam);
	UFUNCTION(BlueprintCallable)
	static void UnregisterEntity(ANPCBaseStateEnemy* Requester,const EEnemyType EnemyType, const FGameplayTag MyTeam);
	UFUNCTION(BlueprintCallable)
	static void UpdateActiveEntity(const EEnemyType EnemyType, const FGameplayTag MyTeam);
	UFUNCTION(BlueprintCallable)
	static void ForceRegisterEntity(ANPCBaseStateEnemy* Requester,const EEnemyType EnemyType, const FGameplayTag MyTeam,AActor* Target);
};
