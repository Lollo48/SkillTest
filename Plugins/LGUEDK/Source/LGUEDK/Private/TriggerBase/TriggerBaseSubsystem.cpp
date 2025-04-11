// Copyright Villains, Inc. All Rights Reserved.


#include "TriggerBase/TriggerBaseSubsystem.h"

#include "EngineUtils.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "TriggerBase/TriggerUtility.h"
#include "Utility/LGDebug.h"

void UTriggerBaseSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UTriggerUtility::Init(this);
	TriggerMap = TMap<FGameplayTag, FTriggerContainer>();
}

void UTriggerBaseSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UTriggerBaseSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	const UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		LGDebug::Error("Inizialize failed because World is not valid", true);
		return;
	}
	
	const AGameModeBase* GameMode = World->GetAuthGameMode();
	
	if (!IsValid(GameMode))
	{
		LGDebug::Error("Inizialize failed because GameMode is not valid", true);
		return;
	}
	
	if (!TriggerManager.Contains(GameMode->GetClass()))
	{
		if (bShowDebug)
			LGDebug::Error("GameMode doesn't exist", true);
		return;
	}
	
	if (bShowDebug)
		LGDebug::Log("GameMode corretta", true);

	Init();
}

void UTriggerBaseSubsystem::SetTriggerMap(ABaseTrigger* OutTriggers)
{
	FGameplayTag ObjectTag = OutTriggers->GetGameplayTag(); 
			
	if (!TriggerMap.Contains(ObjectTag))
	{
		TriggerMap.Add(ObjectTag, FTriggerContainer());
	}
			
	TriggerMap[ObjectTag].Triggers.Add(OutTriggers);
	if (bShowDebug)
	{
		LGDebug::Log("Trigger Map Size: " + FString::FromInt(TriggerMap[ObjectTag].Triggers.Num()), true);
	}

	SetTriggerMapBP(OutTriggers);
}

TArray<ABaseTrigger*> UTriggerBaseSubsystem::GetTriggerContainer(FGameplayTag InGamePlayTag)
{
	if (!TriggerMap.Contains(InGamePlayTag))
	{
		LGDebug::Error("Trigger Map doesn't exist", true);
		return TArray<ABaseTrigger*>();
	}
	
	return TriggerMap[InGamePlayTag].Triggers;
}

void UTriggerBaseSubsystem::Init()
{
	InitBP(); 
}

void UTriggerBaseSubsystem::BindToOnEnable(const FGameplayTag& Name, const FEnableTrigger& Context, bool bUnique)
{
	if (!TriggerMap.Contains(Name))
	{
		LGDebug::Warning("Map does not contain the key", true);
		return;
	}

	FTriggerContainer& TriggerContainer = TriggerMap[Name];

	if (TriggerContainer.Triggers.Num() == 0)
	{
		LGDebug::Warning("TriggerContainer for key " + Name.ToString() + " is empty!", true);
		return;
	}

	TriggerContainer.Triggers[0]->BindToOnEnable(Context,bUnique);
	
}

void UTriggerBaseSubsystem::BindAllToOnEnable(const FGameplayTag& Name,const FEnableTrigger& Context, bool bUnique)
{

	if (!TriggerMap.Contains(Name))
	{
		LGDebug::Warning("Map does not contain the key", true);
		return;
	}
	
	for (auto& Element : TriggerMap[Name].Triggers) 
	{
		if (ABaseTrigger* TriggeredObject = Cast<ABaseTrigger>(Element))
		{
			TriggeredObject->BindToOnEnable(Context,bUnique);
		}
	}
}

void UTriggerBaseSubsystem::UnBindToOnEnable(const FGameplayTag& Name, const FEnableTrigger& Context)
{
	if (!TriggerMap.Contains(Name))
	{
		LGDebug::Warning("Map does not contain the key", true);
		return;
	}

	FTriggerContainer& TriggerContainer = TriggerMap[Name];

	if (TriggerContainer.Triggers.Num() == 0)
	{
		LGDebug::Warning("TriggerContainer for key " + Name.ToString() + " is empty!", true);
		return;
	}

	TriggerContainer.Triggers[0]->UnBindToOnEnable(Context);
	
}

void UTriggerBaseSubsystem::UnBindAllToOnEnable(const FGameplayTag& Name, const FEnableTrigger& Context)
{
	if (!TriggerMap.Contains(Name))
	{
		LGDebug::Warning("Map does not contain the key", true);
		return;
	}
	
	for (auto& Element : TriggerMap[Name].Triggers) 
	{
		if (ABaseTrigger* TriggeredObject = Cast<ABaseTrigger>(Element))
		{
			TriggeredObject->UnBindToOnEnable(Context);
		}
	}
}

void UTriggerBaseSubsystem::BindToOnDisable(const FGameplayTag& Name, const FDisableTrigger& Context, bool bUnique)
{
	if (!TriggerMap.Contains(Name))
	{
		LGDebug::Warning("Map does not contain the key", true);
		return;
	}

	FTriggerContainer& TriggerContainer = TriggerMap[Name];

	if (TriggerContainer.Triggers.Num() == 0)
	{
		LGDebug::Warning("TriggerContainer for key " + Name.ToString() + " is empty!", true);
		return;
	}

	TriggerContainer.Triggers[0]->BindToOnDisable(Context,bUnique);
}

void UTriggerBaseSubsystem::BindAllToOnDisable(const FGameplayTag& Name, const FDisableTrigger& Context, bool bUnique)
{
	if (!TriggerMap.Contains(Name))
	{
		LGDebug::Warning("Map does not contain the key", true);
		return;
	}
	
	for (auto& Element : TriggerMap[Name].Triggers) 
	{
		if (ABaseTrigger* TriggeredObject = Cast<ABaseTrigger>(Element))
		{
			TriggeredObject->BindToOnDisable(Context,bUnique);
		}
	}
}

void UTriggerBaseSubsystem::UnBindToOnDisable(const FGameplayTag& Name, const FDisableTrigger& Context)
{
	if (!TriggerMap.Contains(Name))
	{
		LGDebug::Warning("Map does not contain the key", true);
		return;
	}

	FTriggerContainer& TriggerContainer = TriggerMap[Name];

	if (TriggerContainer.Triggers.Num() == 0)
	{
		LGDebug::Warning("TriggerContainer for key " + Name.ToString() + " is empty!", true);
		return;
	}

	TriggerContainer.Triggers[0]->UnBindToOnDisable(Context);
}

void UTriggerBaseSubsystem::UnBindAllToOnDisable(const FGameplayTag& Name, const FDisableTrigger& Context)
{
	if (!TriggerMap.Contains(Name))
	{
		LGDebug::Warning("Map does not contain the key", true);
		return;
	}
	
	for (auto& Element : TriggerMap[Name].Triggers) 
	{
		if (ABaseTrigger* TriggeredObject = Cast<ABaseTrigger>(Element))
		{
			TriggeredObject->UnBindToOnDisable(Context);
		}
	}
}


