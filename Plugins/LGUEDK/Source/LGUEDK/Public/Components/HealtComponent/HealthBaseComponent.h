// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthBaseComponent.generated.h"

//delegato con due parametri
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FTakeDamage,
	double, Damage,
	double, CurrentHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FDead);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FHeal,
	double, HealAmount,
	double, CurrentHealth);


UCLASS(Blueprintable,BlueprintType,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LGUEDK_API UHealthBaseComponent : public UActorComponent
{
	GENERATED_BODY()

//dichiarazione delegato 
public:
	//assegno da blueprint
	UPROPERTY(BlueprintAssignable)
	FTakeDamage OnTakeDamage;
	UPROPERTY(BlueprintAssignable)
	FDead OnDead;
	UPROPERTY(BlueprintAssignable)
	FHeal OnHeal;

	//EditAnywhere lo posso modificare anche se spawn, meta serve per aggiungere funzionalità categoria menu a tendina
	UPROPERTY(EditAnywhere,meta = (ClampMin = 0),Category = "Health")
	double MaxHealth;
	UPROPERTY(EditAnywhere,meta = (ClampMin = 0),Category = "Health")
	double DefaultHealth;
	
protected:
	bool bIsDead;
	bool bIsInvulnerable;
	bool bIsInitialized;

private:
	double CurrentHealth;
	
public:
	// Sets default values for this component's properties
	UHealthBaseComponent();

	UFUNCTION(BlueprintCallable)
	bool IsInvulnerable() const; // non modifico nessu campo --> il const
	UFUNCTION(BlueprintCallable)
	bool IsDead() const;
	UFUNCTION(BlueprintCallable)
	double GetCurrentHealth() const;
	
	UFUNCTION(BlueprintCallable)
	float SetHealth(double NewHealth);
	UFUNCTION(BlueprintCallable)
	float AddHealth(const double Amount);
	UFUNCTION(BlueprintCallable)
	float RemoveHealth(const double Amount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Initialize();

private:
	void OnDeath();
	void CheckDeath();
	

};
