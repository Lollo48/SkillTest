// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EJumpingActionState : uint8
{
	StartJump UMETA(DisplayName = "StartJump"),
	Jumping UMETA(DisplayName = "Jumping"),
	EndJump UMETA(DisplayName = "EndJump"),
};
