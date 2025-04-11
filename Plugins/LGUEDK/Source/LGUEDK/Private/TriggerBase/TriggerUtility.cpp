// Copyright Villains, Inc. All Rights Reserved.


#include "TriggerBase/TriggerUtility.h"
#include "Utility/LGDebug.h"

UTriggerBaseSubsystem* UTriggerUtility::TriggerSubsystem = nullptr;

void UTriggerUtility::Init(UTriggerBaseSubsystem* InTriggerSubsystem)
{
	if (!IsValid(InTriggerSubsystem))
	{
		LGDebug::Error("TriggerSubsystem is not valid", true);
		return;
	}
	
	TriggerSubsystem = InTriggerSubsystem;
}

void UTriggerUtility::SetTriggerMap(ABaseTrigger* OutTriggers)
{
	TriggerSubsystem->SetTriggerMap(OutTriggers);
}

TArray<ABaseTrigger*> UTriggerUtility::GetTriggerContainer(FGameplayTag InGamePlayTag)
{
	return TriggerSubsystem->GetTriggerContainer(InGamePlayTag);
}

void UTriggerUtility::BindToOnEnable(const FGameplayTag& Name, const FEnableTrigger& Context, bool bUnique)
{
	TriggerSubsystem->BindToOnEnable(Name, Context, bUnique);
}

void UTriggerUtility::BindAllToOnEnable(const FGameplayTag& Name, const FEnableTrigger& Context, bool bUnique)
{
	TriggerSubsystem->BindAllToOnEnable(Name, Context, bUnique);
}

void UTriggerUtility::UnBindToOnEnable(const FGameplayTag& Name, const FEnableTrigger& Context)
{
	TriggerSubsystem->UnBindToOnEnable(Name, Context);
}

void UTriggerUtility::UnBindAllToOnEnable(const FGameplayTag& Name, const FEnableTrigger& Context)
{
	TriggerSubsystem->UnBindAllToOnEnable(Name, Context);
}

void UTriggerUtility::BindToOnDisable(const FGameplayTag& Name, const FDisableTrigger& Context, bool bUnique)
{
	TriggerSubsystem->BindToOnDisable(Name, Context, bUnique);
}

void UTriggerUtility::BindAllToOnDisable(const FGameplayTag& Name, const FDisableTrigger& Context, bool bUnique)
{
	TriggerSubsystem->BindAllToOnDisable(Name, Context, bUnique);
}

void UTriggerUtility::UnBindToOnDisable(const FGameplayTag& Name, const FDisableTrigger& Context)
{
	TriggerSubsystem->UnBindToOnDisable(Name, Context);
}

void UTriggerUtility::UnBindAllToOnDisable(const FGameplayTag& Name, const FDisableTrigger& Context)
{
	TriggerSubsystem->UnBindAllToOnDisable(Name,Context);
}
