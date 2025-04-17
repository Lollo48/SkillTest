// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


UENUM(BlueprintType)
enum class EMovementActionState : uint8
{
	Walking UMETA(DisplayName = "Walking"),
	Running UMETA(DisplayName = "Running"),
	Flying UMETA(DisplayName = "Flying"),
	Swimming UMETA(DisplayName = "Swimming"),
};
