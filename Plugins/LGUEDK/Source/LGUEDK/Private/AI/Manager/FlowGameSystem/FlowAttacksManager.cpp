// Copyright Villains, Inc. All Rights Reserved.


#include "AI/Manager/FlowGameSystem/FlowAttacksManager.h"

#include "AI/Manager/FlowGameSystem/Utility/FlowAttacksUtility.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"
#include "GameFramework/GameModeBase.h"
#include "Utility/LGDebug.h"


void UFlowAttacksManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UFlowAttacksUtility::Init(this);
}

void UFlowAttacksManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	bIsInitialized = false;

	UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("Flow game: Invalid world"));
		return;
	}
	
	const AGameModeBase* GameMode = World->GetAuthGameMode();
	
	if (!IsValid(GameMode))
	{
		UE_LOG(LogTemp, Warning, TEXT("Flow game: Invalid GameMode"));
		return;
	}

	if (!GameModes.Contains(GameMode->GetClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Flow game: Doesn't contain this game mode OnWorldBeginPlay"));
		return;
	}
	Init();
}

void UFlowAttacksManager::Deinitialize()
{
	Super::Deinitialize();
}

void UFlowAttacksManager::Init()
{
	UWorld* World = GetWorld();
	
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("Flow game: Invalid world"));
		return;
	}

	const AGameModeBase* GameMode = World->GetAuthGameMode();
	if (!IsValid(GameMode))
	{
		UE_LOG(LogTemp, Warning, TEXT("Flow game: invalid game mode INIT"));
		return;
	}

	if (!GameModes.Contains(GameMode->GetClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Flow game: Doesn't contain this game mode INIT"));
		return;
	}

	EntitiesLimits.Empty();
	ActiveEntities.Empty();
	PendingEntities.Empty();
	
	const FAttackingTeam* FlowConfig = GameModes.Find(GameMode->GetClass());
	if (!FlowConfig)
	{
		UE_LOG(LogTemp, Warning, TEXT("Flow game: Doesn't contain this FlowConfig"));
		return;
	}
	
	TMap<TEnumAsByte<EEnemyType>, int32> TempEnemyLimits;
	TMap<TEnumAsByte<EEnemyType>, TArray<ANPCBaseStateEnemy*>> TempActiveEntities;
	TMap<TEnumAsByte<EEnemyType>, TArray<ANPCBaseStateEnemy*>> TempPendingEntities;
	
	for (const FTeamData& FlowGameEnemyLimitPerMode : FlowConfig->Teams)
	{
		const FGameplayTag& TeamTag = FlowGameEnemyLimitPerMode.TeamID;

		for (const FMaxEnemiesPerTypeData& FlowGameEnemyList : FlowGameEnemyLimitPerMode.MaxEnemiesPerType)
		{
			EntitiesLimits.Add(TeamTag);
			TempEnemyLimits.Add(FlowGameEnemyList.EnemyType, FlowGameEnemyList.MaxAttackersSimultaneous);
			EntitiesLimits[TeamTag] = TempEnemyLimits;
			
			ActiveEntities.Add(TeamTag);
			TempActiveEntities.Add(FlowGameEnemyList.EnemyType, TArray<ANPCBaseStateEnemy*>());
			ActiveEntities[TeamTag] = TempActiveEntities;
			
			PendingEntities.Add(TeamTag);
			TempPendingEntities.Add(FlowGameEnemyList.EnemyType, TArray<ANPCBaseStateEnemy*>());
			PendingEntities[TeamTag] = TempPendingEntities;
		}
	}

	bIsInitialized = true;
	
	if (bShowDebug)
		DebugEntityMaps();
	
	InitBP();
}

void UFlowAttacksManager::SetPendingEntity(ANPCBaseStateEnemy* Requester)
{
	if (!IsValid(Requester)) return;
	
	ANPCBaseStateEnemyController* Controller = Cast<ANPCBaseStateEnemyController>(Requester->GetInstigatorController());
	if (!Controller) return;
	
	Controller->SetStateAsPending(Requester->GetTarget());
	
	if (bShowDebug)
	{
		const FVector Location = Requester->GetActorLocation() + FVector(0, 0, 100);
		DrawDebugString(GetWorld(), Location, TEXT("PENDING"), nullptr, FColor::Blue, 5000.0f, true);
	}
}

void UFlowAttacksManager::SetActiveEntity(ANPCBaseStateEnemy* Requester)
{
	if (!IsValid(Requester)) return;
	
	ANPCBaseStateEnemyController* Controller = Cast<ANPCBaseStateEnemyController>(Requester->GetInstigatorController());
	if (!Controller) return;
	
	if (Requester->GetEntityState() == EEnemyState::Attacking)return;
		Controller->SetStateAsAttacking(Requester->GetTarget());
	
	if (bShowDebug)
	{
		const FVector Location = Requester->GetActorLocation() + FVector(0, 0, 100);
		DrawDebugString(GetWorld(), Location, TEXT("ACTIVE"), nullptr, FColor::Green, 5000.0f, true);
	}
}

bool UFlowAttacksManager::IsValidMap(const EEnemyType EnemyType, const FGameplayTag MyTeam)
{
	if (!EntitiesLimits.Contains(MyTeam) || !EntitiesLimits[MyTeam].Contains(EnemyType))
	{
		UE_LOG(LogTemp, Warning, TEXT("No limit defined for enemy %s at target %s"), *UEnum::GetValueAsString(EnemyType), *MyTeam.ToString());
		return false;
	}
	
	if (!ActiveEntities.Contains(MyTeam) || !ActiveEntities[MyTeam].Contains(EnemyType))
	{
		UE_LOG(LogTemp, Warning, TEXT("ActiveEntities %s at target %s"), *UEnum::GetValueAsString(EnemyType), *MyTeam.ToString());
		return false;
	}

	if (!PendingEntities.Contains(MyTeam) || !PendingEntities[MyTeam].Contains(EnemyType))
	{
		UE_LOG(LogTemp, Warning, TEXT("PendingEntities %s at target %s"), *UEnum::GetValueAsString(EnemyType), *MyTeam.ToString());
		return false;
	}

	return true;
}

void UFlowAttacksManager::RegisterEntity(ANPCBaseStateEnemy* Requester, const EEnemyType EnemyType, const FGameplayTag MyTeam)
{
	if (!Requester) return;
	if (!IsValidMap(EnemyType,MyTeam))return;
	if (ActiveEntities[MyTeam][EnemyType].Contains(Requester))return;
	if (PendingEntities[MyTeam][EnemyType].Contains(Requester))return;
	if (!bIsInitialized)return;

	const int32 MaxAllowed = EntitiesLimits[MyTeam][EnemyType];
	
	if (ActiveEntities[MyTeam][EnemyType].Num() < MaxAllowed)
	{
		ActiveEntities[MyTeam][EnemyType].AddUnique(Requester);
		SetActiveEntity(Requester);
		return;
	}
	
	PendingEntities[MyTeam][EnemyType].AddUnique(Requester);
	SetPendingEntity(Requester);
}

void UFlowAttacksManager::UnregisterEntity(ANPCBaseStateEnemy* Requester, const EEnemyType EnemyType, const FGameplayTag MyTeam)
{
	if (!IsValid(Requester)) return;
	if (!IsValidMap(EnemyType, MyTeam))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Map - Enemy: %s | Team: %s"), *UEnum::GetValueAsString(EnemyType), *MyTeam.ToString());
		return;
	}
	if (!bIsInitialized)return;
	
	if (ActiveEntities[MyTeam][EnemyType].Contains(Requester))
	{
		ActiveEntities[MyTeam][EnemyType].Remove(Requester);
		UpdateActiveEntity(EnemyType,MyTeam);
		
		if (bShowDebug)
		{
			const FVector Location = Requester->GetActorLocation() + FVector(0, 0, 100);
			DrawDebugString(GetWorld(), Location, TEXT("UNREGISTER"), nullptr, FColor::Red, 5000.0f, true);
		}
		return;
	}

	if (PendingEntities[MyTeam][EnemyType].Contains(Requester))
	{
		PendingEntities[MyTeam][EnemyType].Remove(Requester);
		UpdateActiveEntity(EnemyType,MyTeam);

		if (bShowDebug)
		{
			const FVector Location = Requester->GetActorLocation() + FVector(0, 0, 100);
			DrawDebugString(GetWorld(), Location, TEXT("UNREGISTER"), nullptr, FColor::Red, 5000.0f, true);
		}
	}
}

void UFlowAttacksManager::UpdateActiveEntity(const EEnemyType EnemyType, const FGameplayTag MyTeam)
{
	if (!IsValidMap(EnemyType, MyTeam)) return;
	if (PendingEntities[MyTeam][EnemyType].Num() <= 0) return;
	if (!bIsInitialized)return;

	int32 MaxWeight = -1;
	TArray<ANPCBaseStateEnemy*> Candidates;
	
	for (ANPCBaseStateEnemy* Enemy : PendingEntities[MyTeam][EnemyType])
	{
		int32 Weight = Enemy->GetDataAsset()->AttackWeight;

		if (Weight > MaxWeight)
		{
			MaxWeight = Weight;
			Candidates.Empty();
			Candidates.Add(Enemy);
		}
		else if (Weight == MaxWeight)
		{
			Candidates.Add(Enemy);
		}
	}

	ANPCBaseStateEnemy* EnemyToActivate = nullptr;

	if (Candidates.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, Candidates.Num() - 1);
		EnemyToActivate = Candidates[RandomIndex];
	}
	else
	{
		EnemyToActivate = *PendingEntities[MyTeam][EnemyType].CreateIterator();
	}

	PendingEntities[MyTeam][EnemyType].Remove(EnemyToActivate);
	ActiveEntities[MyTeam][EnemyType].Add(EnemyToActivate);
	SetActiveEntity(EnemyToActivate);

	if (bShowDebug && IsValid(EnemyToActivate))
	{
		const FVector DebugLocation = EnemyToActivate->GetActorLocation() + FVector(0, 0, 100);
		const FString DebugText = FString::Printf(TEXT("ACTIVATED [%s] Weight: %d"),
			*EnemyToActivate->GetName(),
			EnemyToActivate->GetDataAsset()->AttackWeight);

		DrawDebugString(GetWorld(), DebugLocation, DebugText, nullptr, FColor::Green, 5000.0f, true);
	}
}

void UFlowAttacksManager::ForceRegisterEntity(ANPCBaseStateEnemy* Requester, const EEnemyType EnemyType, const FGameplayTag MyTeam, const AActor* Target)
{
	if (!IsValid(Requester)) return;
	if (!IsValidMap(EnemyType,MyTeam))return;
	if (!bIsInitialized)return;
	if (!IsValid(Target)) return;
	
	const int32 MaxAllowed = EntitiesLimits[MyTeam][EnemyType];  
	
	if (ActiveEntities[MyTeam][EnemyType].Num() < MaxAllowed)
	{
		ActiveEntities[MyTeam][EnemyType].AddUnique(Requester);
		SetActiveEntity(Requester);
		return;
	}
	
	ANPCBaseStateEnemy* FarthestEnemy = nullptr;
	float MaxDistance = -1.0f;
	const FVector TargetLocation = Target->GetActorLocation();

	for (ANPCBaseStateEnemy* CurrentEnemy : ActiveEntities[MyTeam][EnemyType])
	{
		if (CurrentEnemy == nullptr || !IsValid(CurrentEnemy)) continue;
		
		const float Distance = FVector::Distance(CurrentEnemy->GetActorLocation(), TargetLocation);
		if (Distance > MaxDistance)
		{
			MaxDistance = Distance;
			FarthestEnemy = CurrentEnemy;
		}
	}
	
	if (IsValid(FarthestEnemy))
	{
		ActiveEntities[MyTeam][EnemyType].Remove(FarthestEnemy);
		PendingEntities[MyTeam][EnemyType].AddUnique(FarthestEnemy);
		SetPendingEntity(FarthestEnemy);

		ActiveEntities[MyTeam][EnemyType].AddUnique(Requester);
		SetActiveEntity(Requester);
	}
}

void UFlowAttacksManager::DebugEntityMaps() const
{
	UE_LOG(LogTemp, Log, TEXT("==== EntitiesLimits ===="));
	for (const auto& TargetPair : EntitiesLimits)
	{
		FGameplayTag MyTeam = TargetPair.Key;
		for (const auto& EnemyLimit : TargetPair.Value)
		{
			FString EnemyName = UEnum::GetValueAsString(EnemyLimit.Key);
			int32 Limit = EnemyLimit.Value;
			UE_LOG(LogTemp, Log, TEXT("Target %s - Enemy %s: Limit %d"), *MyTeam.ToString(), *EnemyName, Limit);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("==== ActiveEntities ===="));
	for (const auto& TargetPair : ActiveEntities)
	{
		FGameplayTag MyTeam = TargetPair.Key;
		for (const auto& EnemyGroup : TargetPair.Value)
		{
			FString EnemyName = UEnum::GetValueAsString(EnemyGroup.Key);
			int32 Count = EnemyGroup.Value.Num();
			UE_LOG(LogTemp, Log, TEXT("Target %s - Enemy %s: Pending %d"), *MyTeam.ToString(), *EnemyName, Count);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("==== PendingEntities ===="));
	for (const auto& TargetPair : PendingEntities)
	{
		FGameplayTag MyTeam = TargetPair.Key;
		for (const auto& EnemyGroup : TargetPair.Value)
		{
			FString EnemyName = UEnum::GetValueAsString(EnemyGroup.Key);
			int32 Count = EnemyGroup.Value.Num();
			UE_LOG(LogTemp, Log, TEXT("Target %s - Enemy %s: Pending %d"), *MyTeam.ToString(), *EnemyName, Count);
		}
	}
}
