// Copyright Villains, Inc. All Rights Reserved.


#include "Components/TeamComponent/EnemyAttackRequestManager.h"

#include "Utility/LGDebug.h"


UEnemyAttackRequestManager::UEnemyAttackRequestManager()
{

}


void UEnemyAttackRequestManager::BeginPlay()
{
	Super::BeginPlay();
	CurrentTokens = MaxTokens;
}


bool UEnemyAttackRequestManager::TryGetToken(int TokenCost)
{
	if (CurrentTokens < TokenCost)
	{
		LGDebug::Log("Not enough tokens. Current tokens:" + CurrentTokens, true);
		return false;
	}
	
	CurrentTokens -= TokenCost;
	//LGDebug::Log(" CurrentTokens: " + CurrentTokens, true);
	//LGDebug::Log(" TokenCost: " + TokenCost, true);
	return true;
}

void UEnemyAttackRequestManager::ReleaseToken(int TokenCost)
{
	if (CurrentTokens >= MaxTokens)
		return;
	
	CurrentTokens += TokenCost;
		
	//LGDebug::Log("TOKEN TORNATO " + CurrentTokens, true);
}

void UEnemyAttackRequestManager::StoreToken(AActor* AttackTarget, int TokenCost)
{
	if (StoredTokens.Contains(AttackTarget))
	{
		StoredTokens[AttackTarget] += TokenCost;
		return;
	}
	
	StoredTokens.Add(AttackTarget);
	StoredTokens[AttackTarget] = TokenCost;
	
	//LGDebug::Log("TOKEN SALVATO " + FString::FromInt(StoredTokens[AttackTarget]), true);
}

int UEnemyAttackRequestManager::GetCorrectAttackToken(AActor* AttackTarget)
{
	int Token = 0;
	
	if (!StoredTokens.Contains(AttackTarget))
		return Token;
	
	Token = StoredTokens[AttackTarget];
	StoredTokens[AttackTarget] = 0;
	
	//LGDebug::Log("TOKEN RIMOSSO " + FString::FromInt(StoredTokens[AttackTarget]), true);
	
	return Token;
}

void UEnemyAttackRequestManager::ReturnAllTokens()
{
	if (StoredTokens.Num() == 0)
		return;
	
	for (const auto& Token : StoredTokens)
	{
		IAITargetInterface* TargetInterface = Cast<IAITargetInterface>(Token.Key);
		if (!TargetInterface)
			continue;

		TargetInterface->ReleaseToken(Token.Value);

		//LGDebug::Log("TOKEN RIMOSSO " + FString::FromInt(Token.Value), true);
	}
}







