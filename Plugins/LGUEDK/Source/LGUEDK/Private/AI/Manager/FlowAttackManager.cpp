// Copyright Villains, Inc. All Rights Reserved.

#include "AI/Manager/FlowAttackManager.h"

#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"
#include "Kismet/GameplayStatics.h"


AFlowAttackManager::AFlowAttackManager()
{
	
}

void AFlowAttackManager::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void AFlowAttackManager::Init()
{
	ActiveEnemies = TMap<EEnemyType,TArray<AActor*>>();
	ActiveEnemies.Add(EEnemyType::Melee);
	ActiveEnemies.Add(EEnemyType::Ranged);
	ActiveEnemies[EEnemyType::Melee] = TArray<AActor*>();
	ActiveEnemies[EEnemyType::Ranged] = TArray<AActor*>();

	PendingEnemies = TMap<EEnemyType,TArray<AActor*>>();
	PendingEnemies.Add(EEnemyType::Melee);
	PendingEnemies.Add(EEnemyType::Ranged);
	PendingEnemies[EEnemyType::Melee] = TArray<AActor*>();
	PendingEnemies[EEnemyType::Ranged] = TArray<AActor*>();

}

void AFlowAttackManager::RegisterEnemy(AActor* Requester,const EEnemyType EnemyType)
{
	if (!Requester) return;
	if (!ActiveEnemies.Contains(EnemyType))
	{
		SetAttackingEnemy(Requester);
		return;
	}
	if (ActiveEnemies[EnemyType].Contains(Requester))return;
	
	const int32 MaxAllowed = (EnemyType == EEnemyType::Melee) ? MaxMeleeAttackers : MaxRangedAttackers;
	
#if !UE_BUILD_SHIPPING
	if (bShowDebug)
	{
		LGDebug::Log("NUMERO MASSIMO DI NEMICI" + FString::FromInt(MaxAllowed),true);
		LGDebug::Log("Aggiunto nemico ai nemici attivi" + FString::FromInt(ActiveEnemies[EnemyType].Num()),true);
	}
#endif
	
	if (ActiveEnemies[EnemyType].Num() < MaxAllowed)
	{
		ActiveEnemies.FindOrAdd(EnemyType).AddUnique(Requester);
		SetAttackingEnemy(Requester);
		
		#if !UE_BUILD_SHIPPING
		if (bShowDebug)
		{
			LGDebug::Log("Aggiunto nemico ai nemici attivi" + FString::FromInt(ActiveEnemies[EnemyType].Num()),true);
			
		}
		#endif
	}
	else
	{

		PendingEnemies.FindOrAdd(EnemyType).AddUnique(Requester);
		SetPendingEnemy(Requester);

		#if !UE_BUILD_SHIPPING
		if (bShowDebug)
		{
			LGDebug::Log("Aggiunto nemico ai nemici in attesa",true);
		}
		#endif
	}

}

void AFlowAttackManager::UnregisterEnemy(AActor* Requester,const EEnemyType EnemyType)
{
	if (!IsValid(Requester)) return;
	
	if (ActiveEnemies.Contains(EnemyType) && ActiveEnemies[EnemyType].Contains(Requester))
	{
		ActiveEnemies[EnemyType].Remove(Requester);
		PendingEnemies[EnemyType].Remove(Requester);
		UpdateAttackers(EnemyType); 
	}
	
}

void AFlowAttackManager::UpdateAttackers(EEnemyType EnemyType)
{
	if (!PendingEnemies.Contains(EnemyType) || PendingEnemies[EnemyType].Num() <= 0)
	{
		return; 
	}
	
	AActor* EnemyToActivate = *PendingEnemies[EnemyType].CreateIterator();
	PendingEnemies[EnemyType].Remove(EnemyToActivate);
	
	ActiveEnemies[EnemyType].Add(EnemyToActivate);
	
	ANPCBaseStateEnemy* Enemy = Cast<ANPCBaseStateEnemy>(EnemyToActivate);
	ANPCBaseStateEnemyController* EnemyController = Cast<ANPCBaseStateEnemyController>(EnemyToActivate->GetInstigatorController());
	AActor* Target = Enemy->GetAttackTarget();
	if (!Target)return;

	EnemyController->SetStateAsAttacking(Target);
	
}

void AFlowAttackManager::ForceRegisterEnemy(AActor* Requester, const EEnemyType EnemyType)
{
	if (!Requester) return;
	
	ANPCBaseStateEnemy* EnemyState = Cast<ANPCBaseStateEnemy>(Requester);
	if (!EnemyState) return;
	
	AActor* Target = EnemyState->GetAttackTarget();
	if (!Target) return;
	
	const int32 MaxAllowed = (EnemyType == EEnemyType::Melee) ? MaxMeleeAttackers : MaxRangedAttackers;
	
	if (ActiveEnemies[EnemyType].Num() < MaxAllowed)
	{
		ActiveEnemies.FindOrAdd(EnemyType).Add(Requester);
		SetAttackingEnemy(Requester);
		return;
	}
	
	AActor* FarthestEnemy = nullptr;
	float MaxDistance = 0.0f;
	FVector PlayerPosition = Target->GetActorLocation();

	for (AActor* CurrentEnemy : ActiveEnemies[EnemyType])
	{
		FVector CurrentEnemyPosition = CurrentEnemy->GetActorLocation();
		float Distance = FVector::Distance(CurrentEnemyPosition, PlayerPosition);
		
		if (Distance > MaxDistance)
		{
			MaxDistance = Distance;
			FarthestEnemy = CurrentEnemy;
		}
	}
	
	if (FarthestEnemy)
	{
		ActiveEnemies[EnemyType].Remove(FarthestEnemy);
		PendingEnemies[EnemyType].AddUnique(FarthestEnemy);
		
		ActiveEnemies[EnemyType].AddUnique(Requester);

		SetPendingEnemy(FarthestEnemy);
		SetAttackingEnemy(Requester);
	}

}

