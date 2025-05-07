// Fill out your copyright notice in the Description page of Project Settings.


#include "AITesting/AITesting.h"


// Sets default values
AAITesting::AAITesting()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAITesting::BeginPlay()
{
	Super::BeginPlay();
	SetIsEnable(true);
}

void AAITesting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



