// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseTrigger.h"
#include "GameplayTagContainer.h"
#include "Data/TriggerContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "TriggerBaseSubsystem.generated.h"

/**
 * 
 */

UCLASS(Abstract,Blueprintable,BlueprintType)
class LGUEDK_API UTriggerBaseSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;
	
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	UFUNCTION(Category = "Trigger")
	void SetTriggerMap(ABaseTrigger* OutTriggers);

	UFUNCTION(BlueprintImplementableEvent,Category = "Trigger")
	void SetTriggerMapBP(ABaseTrigger* OutTriggers);

	UFUNCTION(BlueprintCallable,Category = "Trigger")
	TArray<ABaseTrigger*> GetTriggerContainer(FGameplayTag InGamePlayTag);
	
	virtual void Init();

	UFUNCTION(BlueprintImplementableEvent,Category="Triggerbehavior")
	void InitBP();

	///SE MI VOGLIO ISCRIVERE AD UN TRIGGER SPECIFICO
	UFUNCTION(BlueprintCallable,Blueprintable,  Category = "Trigger")
	void BindToOnEnable(const FGameplayTag& Name, const FEnableTrigger& Context,bool bUnique = true);

	///SE MI VOGLIO ISCRIVERE ALL'EVENTO DI TUTTI I TRIGGER CON LO STESSO GAMEPLAYTAG
	UFUNCTION(BlueprintCallable,Blueprintable,  Category = "Trigger")
	void BindAllToOnEnable(const FGameplayTag& Name,const FEnableTrigger& Context,bool bUnique = true);

	UFUNCTION(BlueprintCallable,Blueprintable,  Category = "Trigger")
	void UnBindToOnEnable(const FGameplayTag& Name, const FEnableTrigger& Context);

	UFUNCTION(BlueprintCallable,Blueprintable,  Category = "Trigger")
	void UnBindAllToOnEnable(const FGameplayTag& Name, const FEnableTrigger& Context);

	///SE MI VOGLIO ISCRIVERE AD UN TRIGGER SPECIFICO
	UFUNCTION(BlueprintCallable,Blueprintable,  Category = "Trigger")
	void BindToOnDisable(const FGameplayTag& Name, const FDisableTrigger& Context,bool bUnique = true);

	///SE MI VOGLIO ISCRIVERE ALL'EVENTO DI TUTTI I TRIGGER CON LO STESSO GAMEPLAYTAG
	UFUNCTION(BlueprintCallable,Blueprintable,  Category = "Trigger")
	void BindAllToOnDisable(const FGameplayTag& Name,const FDisableTrigger& Context,bool bUnique = true);

	UFUNCTION(BlueprintCallable,Blueprintable,  Category = "Trigger")
	void UnBindToOnDisable(const FGameplayTag& Name, const FDisableTrigger& Context);

	UFUNCTION(BlueprintCallable,Blueprintable,  Category = "Trigger")
	void UnBindAllToOnDisable(const FGameplayTag& Name, const FDisableTrigger& Context);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Trigger")
	TArray<TSubclassOf<AGameModeBase>>TriggerManager;
	
	UPROPERTY()
	TMap<FGameplayTag, FTriggerContainer>TriggerMap;

	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bShowDebug;
};

