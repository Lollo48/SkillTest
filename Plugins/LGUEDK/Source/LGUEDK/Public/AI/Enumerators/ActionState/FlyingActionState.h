// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EFlyingActionState : uint8
{
	StartFlying UMETA(DisplayName = "StartFlying"),
	Flying UMETA(DisplayName = "Flying"),
	EndFlying UMETA(DisplayName = "EndFlying"),
};
  