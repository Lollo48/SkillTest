// Copyright Villains, Inc. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum EEnemyType : uint8
{
	Melee UMETA(DisplayName = "Melee"),
	Ranged UMETA(DisplayName = "Ranged"),
	Explosion UMETA(DisplayName = "Explosion"),
	Flying UMETA(DisplayName = "Flying")
};
