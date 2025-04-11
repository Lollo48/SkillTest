#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TriggerbehaviorBase.h"
#include "GameFramework/Actor.h"
#include "BaseTrigger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnableTriggerDispatcher);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisableTriggerDispatcher);

DECLARE_DYNAMIC_DELEGATE(FEnableTrigger);

DECLARE_DYNAMIC_DELEGATE(FDisableTrigger);

UCLASS()
class LGUEDK_API ABaseTrigger : public AActor
{
	GENERATED_BODY()
    
public:    
	ABaseTrigger();

protected:
	
	UPROPERTY(EditDefaultsOnly,Instanced,Category = "Trigger")
	UTriggerbehaviorBase* TriggerBehavior;

	UPROPERTY(EditDefaultsOnly,Category = "Trigger")
	FGameplayTag Name;

	UPROPERTY()
	bool bCanBeTriggered;
	
	virtual void BeginPlay() override;
	
public:
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, Category = "Trigger")
	FEnableTriggerDispatcher OnEnableTriggerDispatcher;

	UPROPERTY(BlueprintAssignable, Category = "Trigger")
	FDisableTriggerDispatcher OnDisableTriggerDispatcher;

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	void EnableTrigger();

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	void DisableTrigger();

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	void OnTriggering(float DeltaTime);

	UFUNCTION()
	virtual void ActivateTotem(){ActivateTotemBP();};

	UFUNCTION()
	virtual void DeactivateTotem(){DeactivateTotemBP();};
	
	UFUNCTION(BlueprintCallable, Category = "Trigger")
	void BindToOnEnable(const FEnableTrigger& Context, bool bUnique);

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	void UnBindToOnEnable(const FEnableTrigger& Context);

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	void BindToOnDisable(const FDisableTrigger& Context, bool bUnique);

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	void UnBindToOnDisable(const FDisableTrigger& Context);
	
	UFUNCTION(BlueprintCallable, Category = "Trigger")
	virtual void Init();

	UFUNCTION(BlueprintImplementableEvent,Category="Triggerbehavior")
	void InitBP();

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	bool IsTriggered() const { return bIsTriggered; }

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	void SetIsTriggered(bool bNewIsTriggered) { bIsTriggered = bNewIsTriggered; }

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	bool CanBeTriggered() const { return bCanBeTriggered; }

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	void SetCanBeTriggered(bool bNewCanBeTriggered) { bCanBeTriggered = bNewCanBeTriggered; }

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	FGameplayTag GetGameplayTag() const { return Name; }


protected:
	
	UFUNCTION(BlueprintImplementableEvent,Category="Triggerbehavior")
	void ActivateTotemBP();
	
	UFUNCTION(BlueprintImplementableEvent,Category="Triggerbehavior")
	void DeactivateTotemBP();
	
private:
	
	UPROPERTY()
	bool bIsTriggered;
	
};
