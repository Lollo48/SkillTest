// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NPCBaseController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "NPCBase.generated.h"

UCLASS()
class LGUEDK_API ANPCBase : public ACharacter
{
	GENERATED_BODY()

public:
	ANPCBase();

	UFUNCTION(Blueprintable,BlueprintCallable)
	UBehaviorTree* GetBehaviorTree() const;

	UFUNCTION(BlueprintCallable, Category = "Init")
	virtual void Init();

	UFUNCTION(BlueprintCallable, Category = "EnableEntity")
	virtual void EnableEntity();

	UFUNCTION(BlueprintCallable, Category = "EnableEntity")
	virtual void EnableEntityEffect() {EnableEntityEffectBP();}

	UFUNCTION(BlueprintCallable, Category = "DisableEntity")
	virtual void DisableEntity();

	UFUNCTION(BlueprintCallable, Category = "DisableEntity")
	virtual void DisableEntityEffect() {DisableEntityEffectBP();}

	UFUNCTION(BlueprintCallable, Category = "Initialize")
	void SetIsInitialize(bool bInit) {bIsInitialize = bInit;}

	UFUNCTION(BlueprintCallable, Category = "Initialize")
	bool GetIsInitialize() const {return bIsInitialize;}

	UFUNCTION(BlueprintCallable, Category = "EnableEntity")
	void SetIsEnable(bool bEnable) {bIsEnable = bEnable;}

	UFUNCTION(BlueprintCallable, Category = "EnableEntity")
	bool GetIsEnable() const {return bIsEnable;}

	UFUNCTION(BlueprintCallable, Category = "Collision")
	void SetEntityCollision();

	UFUNCTION(BlueprintCallable, Category = "Transform")
	void SetEntityTransform();
	
protected:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI",meta=(AllowPrivateAccess="true"))
	UBehaviorTree* BehaviorTree;
	
	UPROPERTY()
	bool bIsInitialize;

	UPROPERTY()
	bool bIsEnable;

	UPROPERTY()
	FVector MeshLocation;
	UPROPERTY()
	FRotator MeshRotation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|CollisionProfileName")
	FName EnemyMeshCollisionProfileName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|CollisionProfileName")
	FName EnemyCapsuleCollisionProfileName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|CollisionProfileName")
	TEnumAsByte<ECollisionChannel> CollisionCapsuleChannel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|CollisionProfileName")
	TEnumAsByte<ECollisionChannel> CollisionMeshChannel;
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent,Category = "Init")
	void InitBP();
	
	UFUNCTION(BlueprintImplementableEvent,Category = "EnableEntity")
	void EnableEntityBP();

	UFUNCTION(BlueprintImplementableEvent,Category = "EnableEntity")
	void EnableEntityEffectBP();
	
	UFUNCTION(BlueprintImplementableEvent,Category = "DisableEntity")
	void DisableEntityBP();
	
	UFUNCTION(BlueprintImplementableEvent,Category = "DisableEntity")
	void DisableEntityEffectBP();
	
};
