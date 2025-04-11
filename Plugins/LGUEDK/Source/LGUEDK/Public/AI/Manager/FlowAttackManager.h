// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enumerators/EEnemyType.h"
#include "AI/EQS/EQS_Manager.h"
#include "GameFramework/Actor.h"
#include "FlowAttackManager.generated.h"

UCLASS()
class LGUEDK_API AFlowAttackManager : public AActor
{
	GENERATED_BODY()

public:
	
	AFlowAttackManager();
	
	void RegisterEnemy(AActor* Requester,const EEnemyType EnemyType);   
	void UnregisterEnemy(AActor* Requester,const EEnemyType EnemyType);                 
	void UpdateAttackers(const EEnemyType EnemyType);                              
	void ForceRegisterEnemy(AActor* Requester,const EEnemyType EnemyType);

protected:
		
	UPROPERTY(EditAnywhere, Category = "Attack Settings")
	int32 MaxMeleeAttackers = 3; 

	UPROPERTY(EditAnywhere, Category = "Attack Settings")
	int32 MaxRangedAttackers = 2; 

	TMap<EEnemyType, TArray<AActor*>> ActiveEnemies;
	TMap<EEnemyType, TArray<AActor*>> PendingEnemies;
	
	virtual void BeginPlay() override;

	UFUNCTION(Blueprintable)
	virtual void SetPendingEnemy(AActor* Requester){}

	UFUNCTION(Blueprintable)
	virtual void SetAttackingEnemy(AActor* Requester){}

private:

	UPROPERTY(EditAnywhere, Category = "Attack Settings")
	bool bShowDebug;

	void Init();
};

