// Copyright Villains, Inc. All Rights Reserved.

#pragma once


UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Passive UMETA(DisplayName = "Passive"),
	Patrolling UMETA(DisplayName = "Patrolling"),
	Investigating UMETA(DisplayName = "Investigating"),
	Flying UMETA(DisplayName = "Flying"),
	Attacking UMETA(DisplayName = "Attacking"),
	Chasing UMETA(DisplayName = "Chasing"),
	Pending UMETA(DisplayName = "Pending"),
	Grabbed UMETA(DisplayName = "Grabbed"),
	Dead UMETA(DisplayName = "Dead"),

};
