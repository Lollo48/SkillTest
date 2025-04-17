// Fill out your copyright notice in the Description page of Project Settings.


#pragma once


UENUM(BlueprintType)
enum class EFlyingMode : uint8
{
	Random UMETA(DisplayName = "Random"),
	Circular UMETA(DisplayName = "Circular"),
	AroundActor UMETA(DisplayName = "AroundActor"),
};
