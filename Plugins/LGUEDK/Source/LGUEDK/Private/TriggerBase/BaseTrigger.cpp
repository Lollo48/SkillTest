// Copyright Villains, Inc. All Rights Reserved.


#include "TriggerBase/BaseTrigger.h"

#include "TriggerBase/TriggerUtility.h"
#include "Utility/LGDebug.h"


// Sets default values
ABaseTrigger::ABaseTrigger()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseTrigger::EnableTrigger(AActor* InTriggerInstigator)
{
	OnEnableTriggerDispatcher.Broadcast();
	TriggerInstigator = InTriggerInstigator;
	
	if (TriggerBehavior == nullptr)
	{
		LGDebug::Log("TriggerBehavior is null", true);
		return;
	}
	SetIsTriggered(true);
	TriggerBehavior->Enable(this);
	ActivateTotem(InTriggerInstigator);
}

void ABaseTrigger::DisableTrigger()
{
	OnDisableTriggerDispatcher.Broadcast();
	if (TriggerBehavior == nullptr)
	{
		LGDebug::Log("TriggerBehavior is null", true);
		return;
	}
	SetCanBeTriggered(true);
	SetIsTriggered(false);
	TriggerBehavior->Disable(this);
	DeactivateTotem();
}

void ABaseTrigger::OnTriggering(float DeltaTime)
{
	if (TriggerBehavior == nullptr)
	{
		LGDebug::Log("TriggerBehavior is null", true);
		return;
	}
	OnTriggeringBP(DeltaTime);
	TriggerBehavior->OnTriggering(DeltaTime,this);
}

void ABaseTrigger::BindToOnEnable(const FEnableTrigger& Context,bool bUnique)
{
	if (bUnique)
	{
		OnEnableTriggerDispatcher.AddUnique(Context);
		return;
	}

	OnEnableTriggerDispatcher.Add(Context);
}

void ABaseTrigger::UnBindToOnEnable(const FEnableTrigger& Context)
{
	OnEnableTriggerDispatcher.Remove(Context);
}

void ABaseTrigger::BindToOnDisable(const FDisableTrigger& Context, bool bUnique)
{
	if (bUnique)
	{
		OnDisableTriggerDispatcher.AddUnique(Context);
		return;
	}

	OnDisableTriggerDispatcher.Add(Context);
}

void ABaseTrigger::UnBindToOnDisable(const FDisableTrigger& Context)
{
	OnDisableTriggerDispatcher.Remove(Context);
}

void ABaseTrigger::Init()
{
	UTriggerUtility::SetTriggerMap(this);
	SetCanBeTriggered(true);
	InitBP();
}

void ABaseTrigger::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void ABaseTrigger::Tick(float DeltaTime)
{
	if (bIsTriggered)
		OnTriggering(DeltaTime);
}



