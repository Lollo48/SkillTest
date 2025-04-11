// Copyright Villains, Inc. All Rights Reserved.


#include "Components/HealtComponent/HealthBaseComponent.h"


//costruttore
UHealthBaseComponent::UHealthBaseComponent()
{
	//se mai utilizzero il tick
	PrimaryComponentTick.bCanEverTick = true;

	bIsDead = false;
	bIsInvulnerable = false;
	
}

bool UHealthBaseComponent::IsInvulnerable() const
{
	return bIsInvulnerable;
}

bool UHealthBaseComponent::IsDead() const
{
	return bIsDead;
}

double UHealthBaseComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

void UHealthBaseComponent::OnDeath()
{
	bIsDead = true;
	CurrentHealth = 0;
	OnDead.Broadcast();
}

void UHealthBaseComponent::CheckDeath()
{
	if (CurrentHealth > 0) return;
	OnDeath();
	
}

float UHealthBaseComponent::SetHealth(double NewHealth)
{
	if (IsInvulnerable()) return CurrentHealth;
	if (CurrentHealth == NewHealth) return CurrentHealth;

	if (CurrentHealth > NewHealth)
	{
		if (IsDead())return CurrentHealth;
		CurrentHealth = NewHealth;
		OnTakeDamage.Broadcast(NewHealth,CurrentHealth);
		CheckDeath();
		return CurrentHealth;
	}

	if (NewHealth > MaxHealth)
	{
		NewHealth = MaxHealth;
	}
	CurrentHealth = NewHealth;
	
	if (IsDead())
		bIsDead = false;
	
	OnHeal.Broadcast(NewHealth,CurrentHealth);
	
	
	return CurrentHealth;
}

float UHealthBaseComponent::AddHealth(const double Amount)
{
	return SetHealth(CurrentHealth + Amount);
}

float UHealthBaseComponent::RemoveHealth(const double Amount)
{
	return SetHealth(CurrentHealth - Amount);
	
}


// Called when the game starts
void UHealthBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void UHealthBaseComponent::Initialize()
{
	if (MaxHealth < 0 || DefaultHealth < 0)
	{
		bIsInitialized = false;
		return;
	}
	
	if (DefaultHealth > MaxHealth)
	{
		DefaultHealth = MaxHealth;
	}
	
	CurrentHealth = DefaultHealth;
	
	CheckDeath();
	
	bIsInitialized = true;
}



